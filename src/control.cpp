#include "control.hpp"

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

ImVec2 parsePosition(std::string x, std::string y) {
  return ImVec2(std::stof(x), std::stof(y));
}

LogData parseLog(std::string line) {
  LogData logData = {};
  std::vector<std::string> tokens;
  split(line, tokens, ',');
  if (tokens.at(1).size() != 1) throw std::runtime_error("Bad type string");
  logData.timestamp = std::stof(tokens.at(0));
  uint8_t type = tokens.at(1).at(0);
  
  float ADJUSTED_HEADING;

  switch(type) {
  case LCPOS:
    logData.type = LogData::LCPos;
    logData.data.pos = parsePosition(tokens.at(2), tokens.at(3));
    break;
  case WRPOS:
    logData.type = LogData::WRPos;
    logData.data.pos = parsePosition(tokens.at(2), tokens.at(3));
    break;
  case WRHEADING:
    ADJUSTED_HEADING = -(std::stof(tokens.at(2)) - PI/2);  // <-- Should not be needed, probably something faulty with algorithm
    logData.type = LogData::WRHeading;
    logData.data.heading = ADJUSTED_HEADING;
    break;
  case WRTARGET:
    logData.type = LogData::WRTarget;
    logData.data.pos = parsePosition(tokens.at(2), tokens.at(3));
    break;
  default: throw std::runtime_error("Bad type character");
  }
  return logData;
}

std::vector<LogData> readLog(std::string fname) {
  std::vector<LogData> logData;
  std::string line;
  
  std::ifstream logFile(fname);
  if (logFile.is_open()) {
    while (std::getline(logFile, line)) {
      try {
	logData.push_back(parseLog(line));
      } catch ( std::exception e ) {
	throw e;
      }
    }
    logFile.close();
  } else throw std::runtime_error("Could not open file");
  return logData;
}

void controlWidget() {
  char buf[128];
  static float lastTime = ImGui::GetTime();
  static float timeScale = 1.0;
  static bool isRunning = false;
  static auto logIterator = logData.begin();

  ImGui::Begin("Control");
  
  if (ImGui::Button("Load log")) {
    state.log.filename = "langedrag-sorted.log";
    logData = readLog(state.log.filename);
    state.log.size = logData.size();
    state.log.time = logData.back().timestamp;
  }; ImGui::SameLine(); ImGui::Text(state.log.filename.c_str());
  char logStat[128];
  sprintf(logStat, "Log items: %d, running time: %.1f s.", state.log.size, state.log.time);
  ImGui::Text(logStat);
  
  if (ImGui::Button("Reset")) {
    isRunning = false;
    logIterator = logData.begin();
    state.time = 0;
  } ImGui::SameLine();

  if (!isRunning) {
    if (ImGui::Button("Start")) {
      isRunning = true;
    }
  } else {
    if (ImGui::Button("Pause")) {
      isRunning = false;
    }
  } ImGui::SameLine();
  ImGui::Text("Time scaling: ");
  ImGui::SameLine(); ImGui::SetNextItemWidth(-1);

  ImGui::DragFloat("##timeScaling", &timeScale, 0.1f, 0, 20, "%.1f");

  float timeDelta = (ImGui::GetTime() - lastTime) * timeScale;
  if (isRunning) {
    state.time += timeDelta;
    if (state.time >= state.log.time) {
      state.time = state.log.time;
      isRunning = false;
    }
  }

  lastTime = ImGui::GetTime();
  
  ImGui::DragFloat("##time", &state.time, 0.1f, 0, state.log.time, "%.1f s");

  ImGui::End();
}
