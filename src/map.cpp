#include "map.hpp"

void mapWidget() {
  static bool trackWR = false;
  static bool trackLC = false;
  static struct {
    float scale = 5.0;
    ImVec2 origin;
  } mapView;
  const float wrSize = 5.0;
  
  ImGui::Begin("WR / LC Map");
    
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  if (ImGui::Button("Center")) mapView.origin = ImVec2(669975.5563621983,6395606.2994977264); ImGui::SameLine();
  //if (ImGui::Button("Center")) {
  //  mapView.origin = ImVec2(state.map.bBox.x.min + ((state.map.bBox.x.max - state.map.bBox.x.min) / 2),
  //			    state.map.bBox.y.min + ((state.map.bBox.y.max - state.map.bBox.y.min) / 2));
  //}ImGui::SameLine();
  if (ImGui::Checkbox("Track LC", &trackLC)) trackWR = false; ImGui::SameLine();
  if (ImGui::Checkbox("Track WR", &trackWR)) trackLC = false;

  ImGui::Text("Scale: %f  Origin: (%f, %f)", mapView.scale, mapView.origin.x, mapView.origin.y);
  ImGui::DragFloat("Scale", &mapView.scale, 0.2f, 1.0f, 72.0f, "%.01f");
  ImGui::DragFloat("Heading", &state.latest.WRHeading, 0.01f, -PI, PI, "%.3f");
    
  ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
  ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    
  if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
  if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
  draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x,
					      canvas_pos.y + canvas_size.y), im_canvasColor);
  draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x,
					canvas_pos.y + canvas_size.y), im_borderColor);
  ImGui::InvisibleButton("canvas", canvas_size);

  // Pan map
  if (ImGui::IsMouseDragging(0)) {
    ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - canvas_pos.x,
					ImGui::GetIO().MousePos.y - canvas_pos.y);
    if (mouse_pos_in_canvas.x >= 0 && mouse_pos_in_canvas.y >= 0 &&
	mouse_pos_in_canvas.x <= canvas_size.x && mouse_pos_in_canvas.y <= canvas_size.y) {
      ImVec2 delta = ImGui::GetIO().MouseDelta;
      mapView.origin.x -= delta.x / mapView.scale;
      mapView.origin.y += delta.y / mapView.scale;
    }
  }
  if (trackLC) mapView.origin = state.latest.LCPos;
  if (trackWR) mapView.origin = state.latest.WRPos;

  draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), true);

  // Plot LC coordinates
  ImVec2 origin(mapView.origin.x, mapView.origin.y);
  ImVec2 this_Pos, prev_LCPos, prev_WRPos;
  bool first_LCPos = true, first_WRPos = true;
  static LogData *pLog_WRHeading;
  static LogData *pLog_WRTarget;
  
  for (auto it = logData.begin(); it != logData.end() && state.time >= it->timestamp; it++) {
    
    if (it->type == LogData::LCPos || it->type == LogData::WRPos || it->type == LogData::WRTarget)
      this_Pos = ImVec2((it->data.pos.x - origin.x) * mapView.scale + canvas_pos.x + canvas_size.x / 2,
			-(it->data.pos.y - origin.y) * mapView.scale + canvas_pos.y + canvas_size.y / 2);
    
    switch (it->type) {
    case LogData::LCPos:
      // this_Pos = ImVec2((it->data.pos.x - origin.x) * mapView.scale + canvas_pos.x + canvas_size.x / 2,
      // 		-(it->data.pos.y - origin.y) * mapView.scale + canvas_pos.y + canvas_size.y / 2);
      draw_list->AddCircleFilled(this_Pos, 3.0f, im_borderColor, 20);
      if
	(first_LCPos) first_LCPos = false;
      else
	draw_list->AddLine(prev_LCPos, this_Pos, im_borderColor, 1.0);
      prev_LCPos = this_Pos;
      break;
      
    case LogData::WRPos:
      // this_Pos = ImVec2((it->data.pos.x - origin.x) * mapView.scale + canvas_pos.x + canvas_size.x / 2,
      //		-(it->data.pos.y - origin.y) * mapView.scale + canvas_pos.y + canvas_size.y / 2);
      if (first_WRPos)
	first_WRPos = false;
      else
	draw_list->AddLine(prev_WRPos, this_Pos, im_focusColor, 1.0);
      prev_WRPos = this_Pos;
      break;
      
    case LogData::WRHeading:
      pLog_WRHeading = &(*it);
      break;

    case LogData::WRTarget:
      pLog_WRTarget = &(*it);
      break;

    }
  }

  // Update state
  state.latest.LCPos = prev_LCPos;
  state.latest.WRPos = prev_WRPos;
  state.latest.WRHeading = (pLog_WRHeading) ? pLog_WRHeading->data.heading : 0.0;
  state.latest.WRTarget = (pLog_WRTarget) ? pLog_WRTarget->data.pos : ImVec2(0,0);
  
  char posBuf[128];
  sprintf(posBuf, "WR: (%.5f, %.5f)  LC: (%.5f, %.5f)", prev_WRPos.x, prev_WRPos.y, prev_LCPos.x, prev_LCPos.y);
  ImGui::SetCursorPos(ImVec2(50, 50));
  //ImGui::TextColored(ImVec4(1,0,0,1), posBuf);
  ImGui::Text(posBuf);

  // Draw WR position
  draw_list->PathLineTo(ImVec2(prev_WRPos.x - 2 * cos(state.latest.WRHeading) * wrSize,
			       prev_WRPos.y - 2 * sin(state.latest.WRHeading) * wrSize));
  draw_list->PathLineTo(ImVec2(prev_WRPos.x + 5 * sin(state.latest.WRHeading) * wrSize,
			       prev_WRPos.y - 5 * cos(state.latest.WRHeading) * wrSize));
  draw_list->PathLineTo(ImVec2(prev_WRPos.x + 2 * cos(state.latest.WRHeading) * wrSize,
			       prev_WRPos.y + 2 * sin(state.latest.WRHeading) * wrSize));
  draw_list->PathLineTo(ImVec2(prev_WRPos.x - 1 * sin(state.latest.WRHeading) * wrSize,
			       prev_WRPos.y + 1 * cos(state.latest.WRHeading) * wrSize));
  draw_list->PathFillConvex(im_focusColor);
  
  draw_list->PopClipRect();
    
  ImGui::End();
}
