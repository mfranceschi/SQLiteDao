#include <sstream>
#include <stdexcept>

class SQLiteError : public std::runtime_error {
public:
  SQLiteError(const std::string &generalMessage,
              const char *sqliteErrMsg = nullptr, int sqliteErrCode = 0)
      : std::runtime_error(makeExceptionMessage(generalMessage, sqliteErrMsg,
                                                sqliteErrCode)) {}

  static std::string makeExceptionMessage(const std::string &generalMessage,
                                          const char *sqliteErrMsg,
                                          int sqliteErrCode) {
    std::ostringstream oss;
    oss << generalMessage;

    if (sqliteErrMsg != nullptr) {
      oss << " - Error message of SQLite is: \"" << sqliteErrMsg << "\"";
    }

    if (sqliteErrCode != 0) {
      oss << " - Error code of SQLite is: \"0x" << std::hex << sqliteErrCode
          << "\"";
    }

    return oss.str();
  }
};
