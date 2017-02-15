// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_ANDROID_SCROLLER_H_
#define UI_GFX_ANDROID_SCROLLER_H_

#include "base/time/time.h"
#include "ui/gfx/gfx_export.h"

namespace gfx {

// Native port of android.widget.Scroller.
// * Change-Id: I4365946f890a76fcfa78ca9d69f2a8e0848095a9
// * Please update the Change-Id as upstream Android changes are pulled.
class GFX_EXPORT Scroller {
 public:
  struct Config {
    Config();

    // Controls fling deceleration. Defaults to 0.015f.
    float fling_friction;

    // Controls fling accumulation. Defaults to disabled.
    bool flywheel_enabled;
  };

  explicit Scroller(const Config& config);
  ~Scroller();

  // Start scrolling by providing a starting point and the distance to travel.
  // The default value of 250 milliseconds will be used for the duration.
  void StartScroll(float start_x,
                   float start_y,
                   float dx,
                   float dy,
                   base::TimeTicks start_time);

  // Start scrolling by providing a starting point, the distance to travel,
  // and the duration of the scroll.
  void StartScroll(float start_x,
                   float start_y,
                   float dx,
                   float dy,
                   base::TimeTicks start_time,
                   base::TimeDelta duration);

  // Start scrolling based on a fling gesture. The distance travelled will
  // depend on the initial velocity of the fling.
  void Fling(float start_x,
             float start_y,
             float velocity_x,
             float velocity_y,
             float min_x,
             float max_x,
             float min_y,
             float max_y,
             base::TimeTicks start_time);

  // Call this when you want to know the new location.  If it returns true,
  // the animation is not yet finished.
  bool ComputeScrollOffset(base::TimeTicks time);

  // Extend the scroll animation by |extend|. This allows a running animation
  // to scroll further and longer when used with |SetFinalX()| or |SetFinalY()|.
  void ExtendDuration(base::TimeDelta extend);
  void SetFinalX(float new_x);
  void SetFinalY(float new_y);

  // Stops the animation. Contrary to |ForceFinished()|, aborting the animation
  // causes the scroller to move to the final x and y position.
  void AbortAnimation();

  // Terminate the scroll without affecting the current x and y positions.
  void ForceFinished(bool finished);

  // Returns whether the scroller has finished scrolling.
  bool IsFinished() const;

  // Returns the time elapsed since the beginning of the scrolling.
  base::TimeDelta GetTimePassed() const;

  // Returns how long the scroll event will take.
  base::TimeDelta GetDuration() const;

  float GetStartX() const;
  float GetStartY() const;
  float GetCurrX() const;
  float GetCurrY() const;
  float GetCurrVelocity() const;
  float GetCurrVelocityX() const;
  float GetCurrVelocityY() const;
  float GetFinalX() const;
  float GetFinalY() const;

  bool IsScrollingInDirection(float xvel, float yvel) const;

 private:
  enum Mode {
    UNDEFINED,
    SCROLL_MODE,
    FLING_MODE,
  };

  void OnDurationChanged();
  void RecomputeDeltas();

  double GetSplineDeceleration(float velocity) const;
  base::TimeDelta GetSplineFlingDuration(float velocity) const;
  double GetSplineFlingDistance(float velocity) const;

  Mode mode_;

  float start_x_;
  float start_y_;
  float final_x_;
  float final_y_;

  float min_x_;
  float max_x_;
  float min_y_;
  float max_y_;

  float curr_x_;
  float curr_y_;
  base::TimeTicks start_time_;
  base::TimeTicks curr_time_;
  base::TimeDelta duration_;
  double duration_seconds_reciprocal_;
  float delta_x_;
  float delta_x_norm_;
  float delta_y_;
  float delta_y_norm_;
  bool finished_;
  bool flywheel_enabled_;

  float velocity_;
  float curr_velocity_;
  float distance_;

  float fling_friction_;
  float deceleration_;
  float tuning_coeff_;
};

}  // namespace gfx

#endif  // UI_GFX_ANDROID_SCROLLER_H_
