/**
 * @file outcome.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Gives common definition of Outcome type.
 * @version 0.1
 * @date 2024-05-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "common/defines.hpp"
namespace CORE_NAMESPACE {

/**
 * @brief Class denoting outcome status of particular operation. Usually used
 * for functions that can fail, but which otherwise return no data or error.
 */
class Outcome {
  public:
    /**
     * @brief Returns true if outcome given by an operation indicates success.
     */
    bool succeeded() const { return _outcome_state == true; }
    /**
     * @brief Returns true if outcome given by an operation indicates failure.
     */
    bool failed() const { return _outcome_state == false; }

    /**
     * @brief Denotes successfully completed operation.
     */
    const static Outcome Successful;
    /**
     * @brief Denotes failed operation.
     */
    const static Outcome Failed;

  protected:
    Outcome(const bool outcome_value) : _outcome_state(outcome_value) {}

  private:
    bool _outcome_state = false;
};

inline const Outcome Outcome::Successful { true };
inline const Outcome Outcome::Failed { false };

} // namespace CORE_NAMESPACE