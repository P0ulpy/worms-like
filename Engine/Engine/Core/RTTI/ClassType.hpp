//
// Created by Flo on 14/12/2022.
//

#ifndef PATHFINDER_RTTI_HPP
#define PATHFINDER_RTTI_HPP

#include <string>
#include <utility>

#define DECLARE_CLASS_TYPE(ClassName, ParentType) \
    static Engine::ClassType* getClassType() { static Engine::ClassType classType{#ClassName, ParentType::getClassType()}; return &classType; } \
    virtual Engine::ClassType* getType() { return getClassType(); }

namespace Engine
{
    class ClassType
    {
    public:
        using ClassTypeName = std::string;

        inline ClassType(ClassTypeName className, ClassType* parentClassType)
            : m_typeName(std::move(className))
            , m_parentClassType(parentClassType)
        {}

        [[nodiscard]] inline const ClassTypeName& getTypeName() const { return m_typeName; }
        [[nodiscard]] inline const ClassType* getParentType() const { return m_parentClassType; }

    private:
        ClassTypeName m_typeName;
        ClassType* m_parentClassType;
    };

    class IClassType
    {
    public:
        virtual ~IClassType() = default;
        virtual ClassType* getType() = 0;
    };

    class NoClassTypeAncestor
    {
    public:
        static ClassType* getClassType() { return nullptr; }
    };
}

#endif //PATHFINDER_RTTI_HPP
