#pragma once

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/serialization/serialization.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

struct Message {
  size_t fragment_id{0}, offset{0};
  int val{0};

  explicit Message(size_t fragment_id, size_t offset, int val);

  [[nodiscard]] auto toJson() const -> std::string;

  explicit Message(const std::string &json);

  [[nodiscard]] auto toString() const;
};
