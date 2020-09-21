#ifndef UTILS_HH
#define UTILS_HH

#include <cassert>
#include <iostream>

#define PRINT_CONTAINER_START()                                                \
  utils::Dumper::printTabs();                                                  \
  os << "{" << std::endl;                                                      \
  utils::Dumper::addTabSpaces(1)

#define PRINT_FIELD(fieldprint, field)                                         \
  utils::Dumper::printTabs();                                                  \
  os << (fieldprint) << (field) << "," << std::endl

#define PRINT_CONTAINER_END()                                                  \
  utils::Dumper::subTabSpaces(1);                                              \
  utils::Dumper::printTabs();                                                  \
  os << "}"

#define PRINT_CONTAINER_ENDL()                                                 \
  utils::Dumper::subTabSpaces(1);                                              \
  utils::Dumper::printTabs();                                                  \
  os << "}," << std::endl
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

class Dumper {
private:
  inline static unsigned m_tab_space{0};
  inline static int m_dumper_count{0};
  std::streambuf *m_backup{nullptr};

public:
  Dumper() = default;
  explicit Dumper(const std::ios &stream = std::cout) {
    assert(m_dumper_count == 0);
    m_backup = std::cout.rdbuf();
    std::cout.rdbuf(stream.rdbuf());
    m_dumper_count++;
  }

  /// Adds tab spaces
  static inline void addTabSpaces(unsigned tabs = 1) {
    assert(m_tab_space >= 0);
    m_tab_space += tabs;
  }

  /// Subtracts tab spaces
  static inline void subTabSpaces(unsigned tabs = 1) {
    m_tab_space -= tabs;
    assert(m_tab_space >= 0);
  }

  /// Get Tab Spaces
  [[nodiscard]] static auto getTabSpaces(void) -> unsigned {
    return m_tab_space;
  }

  /// Print tabs to the desired out stream
  static void printTabs() {
    for (unsigned i = 0; i < m_tab_space; i++) {
      std::cout << "\t";
    }
  }

  virtual ~Dumper() {
    m_dumper_count--;
    m_tab_space = 0;
    std::cout.rdbuf(m_backup);
  }
};
} // namespace utils

#endif // UTILS_HH
