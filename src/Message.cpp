
#include "Message.hpp"


Message::Message(size_t fragment_id, size_t offset, int val)
    : fragment_id(fragment_id), offset(offset), val(val) {}

auto Message::toJson() const -> std::string {
  ptree pt;
  pt.put("offset", offset);
  pt.put("fragment_id", fragment_id);
  pt.put("val", val);

  std::ostringstream buff;
  write_json(buff, pt, false);
  std::string json = buff.str();
  return json;
}

Message::Message(const std::string &json) {
  ptree pt;
  std::istringstream is(json);
  read_json(is, pt);
  offset = pt.get<decltype(offset)>("offset");
  fragment_id = pt.get<decltype(fragment_id)>("fragment_id");
  val = pt.get<decltype(val)>("val");
}

auto Message::toString() const {
  return (boost::format("Fragment: %s\nOffset: %s\nValue:%s") % fragment_id
          % offset % val)
      .str();
}
