#pragma once

#include <map>

namespace GlobeRenderer
{
    enum class FailureType
    {
        COULD_NOT_LINK_SHADER,
        COULD_NOT_PARSE_SHADER,
        COULD_NOT_BIND_SHADER,
        COULD_NOT_LOAD_SHADER,
        COULD_NOT_FIND_UNIFORM_LOCATION,
        COULD_NOT_LOAD_TEXTURE
    };

    enum class BehaviourType
    {
        CONTINUE,
        EXIT,
        ASSERT_OR_CONTINUE,
        ASSERT_OR_EXIT,
    };

    class FailureBehaviour
    {
      public:
        FailureBehaviour() = delete;

        static void Failure(FailureType type);

      private:
        static const std::map<FailureType, BehaviourType> FAILURE_TO_BEHAVIOUR;
    };

}