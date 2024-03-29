#include "core/logger.h"

#include "core/util.h"

std::string Logger::pullLogs() {
  std::string out = '\t' + Time::getTimestamp();
  out.push_back('\n');
  out.insert(out.end(), log_text.begin(), log_text.end());
  out.push_back('\n');

  log_text.clear();
  return out;
}

bool Logger::empty() const {
  return log_text.empty();
}

void Logger::log(const std::string& message) {
  const char* white = " \n\t";
  log_text.insert(log_text.end(),
                  message.begin() + message.find_first_not_of(white),
                  message.begin() + message.find_last_not_of(white) + 1);
  log_text.push_back('\n');
  std::cout << message + '\n';
}