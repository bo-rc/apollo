/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "modules/dreamview/backend/util/json_util.h"

#include "google/protobuf/util/json_util.h"
#include "modules/common/log.h"

namespace apollo {
namespace dreamview {
namespace util {
namespace {

using Json = nlohmann::json;

google::protobuf::util::JsonOptions JsonOption() {
  google::protobuf::util::JsonOptions json_option;
  json_option.always_print_primitive_fields = true;
  return json_option;
}

}  // namespace

std::string JsonUtil::ProtoToTypedJson(const std::string &json_type,
                                       const google::protobuf::Message &proto) {
  static const auto kJsonOption = JsonOption();
  std::string json_string;
  google::protobuf::util::MessageToJsonString(proto, &json_string, kJsonOption);

  Json json_obj;
  json_obj["type"] = json_type;
  json_obj["data"] = Json::parse(json_string);
  return json_obj.dump();
}

bool JsonUtil::GetStringFromJson(const Json &json, const std::string &key,
                                 std::string *value) {
  const auto iter = json.find(key);
  if (iter == json.end()) {
    AERROR << "The json has no such key: " << key;
    return false;
  }
  if (!iter->is_string()) {
    AERROR << "The value of json." << key << " is not a string";
    return false;
  }
  *value = *iter;
  return true;
}

}  // namespace util
}  // namespace dreamview
}  // namespace apollo