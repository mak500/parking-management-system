#ifndef UTILS_HH
#define UTILS_HH

#include <cassert>

namespace utils {
enum Status { UNAVAILABLE, OK, STATUS_COUNT };

template <typename T> class StatusOr {
private:
  Status m_status{Status::UNAVAILABLE};
  T m_data;

public:
  StatusOr() = default;
  explicit StatusOr(Status status) : m_status(status) {}
  explicit StatusOr(const T &data) : m_status(Status::OK), m_data(data) {}
  [[nodiscard]] inline auto isOk() const -> bool {
    return m_status == Status::OK;
  }
  [[nodiscard]] inline auto getData() const -> T {
    assert(isOk());
    return m_data;
  }
  void setData(const T &data) {
    m_status = Status::OK;
    m_data = data;
  }
};
} // namespace utils

#endif // UTILS_HH