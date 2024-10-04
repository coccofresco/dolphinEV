// Copyright 2023 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "Common/Logging/Log.h"
#include "Common/VR/VRFramebuffer.h"

#define _USE_MATH_DEFINES
#include <cassert>
#include <cmath>

#define GL(func) func;
#define OXR(func) func;

namespace Common::VR
{
enum
{
  MaxLayerCount = 2
};
enum
{
  MaxNumEyes = 2
};

enum PlatformFlag
{
  PLATFORM_CONTROLLER_PICO,
  PLATFORM_CONTROLLER_QUEST,
  PLATFORM_EXTENSION_INSTANCE,
  PLATFORM_EXTENSION_PERFORMANCE,
  PLATFORM_TRACKING_FLOOR,
  PLATFORM_MAX
};

typedef union
{
  XrCompositionLayerProjection projection;
  XrCompositionLayerCylinderKHR cylinder;
} CompositorLayer;

#ifdef ANDROID
typedef struct
{
  jobject activity;
  JNIEnv* env;
  JavaVM* vm;
} vrJava;
#endif

class Base
{
public:
  void Init(void* system, const char* name, int version);
  void Destroy();
  void EnterVR();
  void LeaveVR();
  void UpdateFakeSpace(XrReferenceSpaceCreateInfo* space_info);
  void UpdateStageSpace(XrReferenceSpaceCreateInfo* space_info);
  void WaitForFrame();

  bool GetPlatformFlag(PlatformFlag flag) { return m_platform_flags[flag]; }
  void SetPlatformFlag(PlatformFlag flag, bool value) { m_platform_flags[flag] = value; }

  XrInstance GetInstance() { return m_instance; }
  XrSession GetSession() { return m_session; }
  XrSystemId GetSystemId() { return m_system_id; }

  XrSpace GetCurrentSpace() { return m_current_space; }
  XrSpace GetFakeSpace() { return m_fake_space; }
  XrSpace GetHeadSpace() { return m_head_space; }
  XrSpace GetStageSpace() { return m_stage_space; }
  void SetCurrentSpace(XrSpace space) { m_current_space = space; }

  XrTime GetPredictedDisplayTime() { return m_predicted_display_time; }

  int GetMainThreadId() { return m_main_thread_id; }
  int GetRenderThreadId() { return m_render_thread_id; }

private:
  XrInstance m_instance;
  XrSession m_session;
  XrSystemId m_system_id;

  XrSpace m_current_space;
  XrSpace m_fake_space;
  XrSpace m_head_space;
  XrSpace m_stage_space;

  XrTime m_predicted_display_time;

  int m_main_thread_id;
  int m_render_thread_id;

  bool m_platform_flags[PLATFORM_MAX];
  bool m_initialized = false;
};
}  // namespace Common::VR
