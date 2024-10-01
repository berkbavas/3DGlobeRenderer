#include "FailureBehaviour.h"

#include "Util/Logger.h"

using namespace GlobeRenderer;

void FailureBehaviour::Failure(FailureType type)
{
    if (const auto it = FAILURE_TO_BEHAVIOUR.find(type); it != FAILURE_TO_BEHAVIOUR.end())
    {
        const auto behaviour = it->second;

        switch (behaviour)
        {
            case BehaviourType::CONTINUE:
                break;
            case BehaviourType::EXIT:
                LOG_FATAL("FailureBehaviour::Failure: An unrecoverable failure occured. Exiting...");
                std::exit(EXIT_FAILURE);
                break;
            case BehaviourType::ASSERT_OR_CONTINUE:
                BR_ASSERT(false);
                break;
            case BehaviourType::ASSERT_OR_EXIT:
                BR_ASSERT_OR_EXIT(false, "FailureBehaviour::Failure: An unrecoverable failure occured. Exiting...");
                break;
            default:
                BR_ASSERT_OR_EXIT(false, "FailureBehaviour::Failure: Could not find BehaviourType for corresponding FailureType.");
                break;
        }
    }
    else
    {
        LOG_FATAL("FailureBehaviour::Failure: Could not find BehaviourType for corresponding FailureType.");
        std::exit(EXIT_FAILURE);
    }
}

const std::map<FailureType, BehaviourType> FailureBehaviour::FAILURE_TO_BEHAVIOUR = //
    {
        {FailureType::COULD_NOT_LINK_SHADER, BehaviourType::EXIT},
        {FailureType::COULD_NOT_PARSE_SHADER, BehaviourType::EXIT},
        {FailureType::COULD_NOT_BIND_SHADER, BehaviourType::EXIT},
        {FailureType::COULD_NOT_LOAD_SHADER, BehaviourType::EXIT},
        {FailureType::COULD_NOT_FIND_UNIFORM_LOCATION, BehaviourType::CONTINUE},
        {FailureType::COULD_NOT_LOAD_TEXTURE, BehaviourType::EXIT},
};