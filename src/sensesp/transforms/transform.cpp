#include "sensesp.h"

#include "transform.h"

#include <utility>

#include "ArduinoJson.h"

namespace sensesp {

// Transform

std::set<TransformBase*> TransformBase::transforms_;

TransformBase::TransformBase(const String& config_path)
    : Configurable{std::move(config_path)} {
  transforms_.insert(this);
}

}  // namespace sensesp
