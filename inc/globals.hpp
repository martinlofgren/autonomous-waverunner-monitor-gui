#pragma once

#include <vector>

#define PI 3.14159265

extern const ImColor im_canvasColor;
extern const ImColor im_borderColor;
extern const ImColor im_focusColor;
extern const ImColor im_red;
extern const sf::Color sf_bgColor;

struct State {
  float time;
  struct {
    std::string filename;
    int size;
    float time;
  } log;
  struct {
    ImVec2 LCPos;
    ImVec2 WRPos;
    float WRHeading;
    ImVec2 WRTarget;
  } latest;
};
extern State state;

struct LogData {
  float timestamp;
  enum {
    LCPos,
    WRPos,
    WRHeading,
    WRTarget,
    throttle,
    nozzle_angle
  } type;
  union {
    ImVec2 pos;
    float heading;
    float throttle;
    float nozzle_angle;
  } data;
};

extern std::vector<LogData> logData;
