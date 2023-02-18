#pragma once

#include <array>
#include <vector>
#include <functional>
#include <stdexcept>
#include <cmath>

namespace Maths::Graph {
    template <typename NT>
    class BinaryTree;
    template <typename NT>
    class KdTree;

    template <typename CT>
    class GraphNode
    {
    public:
        using ContentT = CT;

        bool operator==(const GraphNode& Other)
        {
            return m_Content == Other.m_Content;
        }

        virtual ~GraphNode() = default;

        GraphNode(const GraphNode& Node) = default;
        GraphNode& operator=(const GraphNode& Node) = default;

        ContentT& GetContent()
        {
            return m_Content;
        }

        ContentT& GetContent() const
        {
            return m_Content;
        }
    protected:
        explicit GraphNode(ContentT Content) : m_Content(Content) {};

        mutable ContentT m_Content;
    };

    template <typename CT, typename NT>
    class TreeNode : public GraphNode<CT>
    {
    public:
        using NodeType = NT;
        using ConstNodeTypePtr = const NodeType*;
        using NodeTypePtr = NodeType*;

        ConstNodeTypePtr GetParent() const
        {
            return m_Parent;
        }
    protected:
        using GraphNode<CT>::GraphNode;

        NodeTypePtr m_Parent = nullptr;

        NodeTypePtr& GetParent()
        {
            return m_Parent;
        }
    };

    template <typename CT>
    class BinaryTreeNode : public TreeNode<CT, BinaryTreeNode<CT>>
    {
    public:
        using NodeType = BinaryTreeNode;
        using ConstNodeTypePtr = const NodeType*;
        using NodeTypePtr = NodeType*;

    public:
        ConstNodeTypePtr GetLeftChild() const
        {
            return m_LeftChild;
        }

        ConstNodeTypePtr GetRightChild() const
        {
            return m_RightChild;
        }

    protected:
        using TreeNode<CT, BinaryTreeNode<CT>>::TreeNode;

        NodeTypePtr m_LeftChild = nullptr;
        NodeTypePtr m_RightChild = nullptr;

        NodeTypePtr& GetLeftChild()
        {
            return m_LeftChild;
        }

        NodeTypePtr& GetRightChild()
        {
            return m_RightChild;
        }

        friend BinaryTree<BinaryTreeNode>;
        friend KdTree<BinaryTreeNode>;
    };

    template <typename NT>
    class Tree : public std::vector<NT*>
    {
    public:
        using NodeType = NT;
        using NodeTypePtr = NodeType*;

        NodeTypePtr GetRoot() const
        {
            return m_Root;
        }

        void Reset()
        {
            for (auto It = this->rbegin(); It != this->rend(); It++)
            {
                delete *It;
            }
            this->clear();
        }

        virtual ~Tree()
        {
            Reset();
        }
    protected:
        NodeTypePtr m_Root = nullptr;
    };

    template <typename NT>
    class BinaryTree : public Tree<NT>
    {
    public:
        using NodeType = NT;
        using NodeTypePtr = NodeType*;
        using NodePositionFn = std::function<NodeTypePtr&(NodeTypePtr)>;
        template <typename Position>
        struct NodePosition {
            static Position Get()
            {
                return Position();
            }
        };
        struct NodePositionLeft : public NodePosition<NodePositionLeft>, public NodePositionFn {
            using NodePositionFn::NodePositionFn;
            static NodePositionLeft Get()
            {
                return NodePositionLeft([](NodeTypePtr Node)->NodeTypePtr& {return Node->GetLeftChild();});
            }
        };
        struct NodePositionRight : public NodePosition<NodePositionLeft>, public NodePositionFn {
            using NodePositionFn::NodePositionFn;
            static NodePositionRight Get()
            {
                return NodePositionRight([](NodeTypePtr Node)->NodeTypePtr& {return Node->GetRightChild();});
            }
        };

        BinaryTree() = default;

        template <typename Position>
        NodeTypePtr AddNode(
            NodeType::ContentT Content,
            NodeTypePtr Parent,
            Position OnPosition = Position::Get()
        )
        {
            return AddNode_Impl(Content, Parent, OnPosition);
        }
    protected:
        NodeTypePtr AddNode_Impl(NodeType::ContentT Content, NodeTypePtr Parent, NodePositionFn OnPosition)
        {
            if (nullptr == Parent) {
                if (nullptr != this->m_Root)
                {
                    throw std::runtime_error("Cannot add node because parent is nullptr and tree already has root");
                }
                auto Root = new NodeType(Content);
                this->m_Root = Root;
                this->push_back(Root);
                return Root;
            } else {
                auto& Child = OnPosition(Parent);
                if (nullptr != Child)
                {
                    throw std::runtime_error("Cannot add child to parent because parent already has child");
                }
                Child = new NodeType(Content);
                auto ParentIt = std::find(this->begin(), this->end(), Parent);
                if (this->end() == ParentIt)
                {
                    throw std::runtime_error("Cannot add node because parent is not in tree");
                }
                Child->m_Parent = Parent;
                this->insert(++ParentIt, Child);
                return Child;
            }
        }
    };

    template <size_t Dimensions>
    size_t GetAxisIndexForKdTreeNode(size_t Depth)
    {
        return (size_t) std::trunc(Depth / Dimensions) % Dimensions;
    }

    template <typename NT>
    class KdTree : public BinaryTree<NT>
    {
    public:
        using NodeType = NT;
        using NodeTypePtr = NodeType*;

        KdTree() = default;

        template <size_t Dimensions, bool AddLastNodes = true>
        void CreateKdTree(
            std::array<std::vector<typename NodeType::ContentT>, Dimensions>& DataToBeOrdered
        )
        {
            this->Reset();
            this->reserve(DataToBeOrdered[0].size());
            for (size_t i = 0; i < Dimensions; i++)
            {
                std::sort(DataToBeOrdered[i].begin(), DataToBeOrdered[i].end(), [&i](
                    const NodeType::ContentT& Content1,
                    const NodeType::ContentT& Content2
                ) {
                    return Content1.GetKdTreePosition()[i] < Content2.GetKdTreePosition()[i];
                });
            }

            std::array<typename std::vector<typename NodeType::ContentT>::iterator, Dimensions> StartIters = {};
            std::array<typename std::vector<typename NodeType::ContentT>::iterator, Dimensions> EndIters = {};

            for (size_t i = 0; i < Dimensions; i++)
            {
                StartIters[i] = DataToBeOrdered[i].begin();
                EndIters[i] = DataToBeOrdered[i].end();
            }

            CreateKdTreeNode<typename BinaryTree<NT>::NodePositionLeft, Dimensions, AddLastNodes>(
                StartIters,
                EndIters
            );
        }

        template <size_t Dimensions>
        NodeTypePtr FindNode(
            NodeType::ContentT& Content,
            NodeTypePtr FromNode = nullptr,
            size_t Depth = Dimensions - 1
        )
        {
            if (nullptr == FromNode)
            {
                FromNode = this->m_Root;
            }

            if (FromNode->GetContent() == Content)
            {
                return FromNode;
            }

            auto& Position = Content.GetKdTreePosition();
            auto& NodePosition = FromNode->GetContent().GetKdTreePosition();
            auto OnIndex = GetAxisIndexForKdTreeNode<Dimensions>(Depth);
            if (Position[OnIndex] <= NodePosition[OnIndex] && nullptr != FromNode->GetLeftChild())
            {
                auto OnLeftNode = FindNode<Dimensions>(Content, FromNode->GetLeftChild(), Depth + 1);
                if (nullptr != OnLeftNode || Position[OnIndex] < NodePosition[OnIndex]) {
                    return OnLeftNode;
                }
            }

            if (Position[OnIndex] >= NodePosition[OnIndex] && nullptr != FromNode->GetRightChild())
            {
                auto OnRightNode = FindNode<Dimensions>(Content, FromNode->GetRightChild(), Depth + 1);
                return OnRightNode;
            }

            return nullptr;
        }
    protected:
        template <typename NodePosition, size_t Dimensions, bool AddLastNodes = true>
        NodeTypePtr CreateKdTreeNode(
            std::array<typename std::vector<typename NodeType::ContentT>::iterator, Dimensions> OnAxisStart,
            std::array<typename std::vector<typename NodeType::ContentT>::iterator, Dimensions> OnAxisEnd,
            NodeTypePtr Parent = nullptr,
            // Dimensions - 1 to start with only 1 line and then cycle
            size_t Depth = Dimensions - 1
        )
        {
            const size_t SelectedIdx = Maths::Graph::GetAxisIndexForKdTreeNode<Dimensions>(Depth);
            const auto& DistanceBetweenStartEnd = std::distance(OnAxisStart[SelectedIdx], OnAxisEnd[SelectedIdx]);
            if (DistanceBetweenStartEnd < 2)
            {
                if constexpr (AddLastNodes) {
                    if (DistanceBetweenStartEnd == 1) {
                        this->template AddNode<NodePosition>(*(OnAxisStart[SelectedIdx]), Parent);
                    }
                }
                return nullptr;
            }

            const typename std::vector<typename NodeType::ContentT>::difference_type Index = std::floor(DistanceBetweenStartEnd / 2);
            auto NewNode = this->template AddNode<NodePosition>(
                *(OnAxisStart[SelectedIdx] + Index),
                Parent
            );

            {
                auto NewAxisEnd = OnAxisEnd;
                NewAxisEnd[SelectedIdx] = OnAxisStart[SelectedIdx] + Index;
                CreateKdTreeNode<typename BinaryTree<NT>::NodePositionLeft, Dimensions, AddLastNodes>(
                    OnAxisStart,
                    NewAxisEnd,
                    NewNode,
                    Depth + 1
                );
            }
            {
                auto NewAxisStart = OnAxisStart;
                NewAxisStart[SelectedIdx] += Index + 1;
                CreateKdTreeNode<typename BinaryTree<NT>::NodePositionRight, Dimensions, AddLastNodes>(
                    NewAxisStart,
                    OnAxisEnd,
                    NewNode,
                    Depth + 1
                );
            }
            return NewNode;
        }
    };
} // Graph // Maths
