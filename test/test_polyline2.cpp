/*
 * Copyright (c) 2019, The Regents of the University of California (Regents).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *    3. Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Please contact the author(s) of this library if you have any questions.
 * Authors: David Fridovich-Keil   ( dfk@eecs.berkeley.edu )
 */

///////////////////////////////////////////////////////////////////////////////
//
// Tests for Polyline2.
//
///////////////////////////////////////////////////////////////////////////////

#include <ilqgames/geometry/polyline2.h>
#include <ilqgames/utils/types.h>

#include <gtest/gtest.h>
#include <math.h>

using namespace ilqgames;

// Check that we find the correct closest point.
TEST(Polyline2Test, ClosestPointWorks) {
  const Point2 p1(0.0, -1.0);
  const Point2 p2(0.0, 1.0);
  const Point2 p3(2.0, 1.0);
  const Polyline2 polyline({p1, p2, p3});
  float signed_squared_distance;
  LineSegment2 segment(p1, p2);
  bool is_vertex;

  // Pick points to the right of the polyline.
  Point2 query(1.0, -2.0);
  Point2 closest = polyline.ClosestPoint(query, &is_vertex, &segment,
                                         &signed_squared_distance);
  EXPECT_TRUE(is_vertex);
  EXPECT_TRUE(closest.isApprox(p1));
  EXPECT_NEAR(signed_squared_distance, 2.0, constants::kSmallNumber);

  query << 0.5, 0.0;
  closest = polyline.ClosestPoint(query, &is_vertex, &segment,
                                  &signed_squared_distance);
  EXPECT_FALSE(is_vertex);
  EXPECT_LT(closest.squaredNorm(), constants::kSmallNumber);
  EXPECT_NEAR(signed_squared_distance, 0.5 * 0.5, constants::kSmallNumber);

  query << 1.5, 0.0;
  closest = polyline.ClosestPoint(query, &is_vertex, &segment,
                                  &signed_squared_distance);
  EXPECT_FALSE(is_vertex);
  EXPECT_TRUE(closest.isApprox(Point2(1.5, 1.0)));
  EXPECT_NEAR(signed_squared_distance, 1.0, constants::kSmallNumber);

  query << 3.0, 0.0;
  closest = polyline.ClosestPoint(query, &is_vertex, &segment,
                                  &signed_squared_distance);
  EXPECT_TRUE(is_vertex);
  EXPECT_TRUE(closest.isApprox(p3));
  EXPECT_NEAR(signed_squared_distance, 2.0, constants::kSmallNumber);

  // Pick points to the left of the polyline.
  query << -1.0, -2.0;
  closest = polyline.ClosestPoint(query, &is_vertex, &segment,
                                  &signed_squared_distance);
  EXPECT_TRUE(is_vertex);
  EXPECT_TRUE(closest.isApprox(p1));
  EXPECT_NEAR(signed_squared_distance, -2.0, constants::kSmallNumber);

  query << -1.0, 0.0;
  closest = polyline.ClosestPoint(query, &is_vertex, &segment,
                                  &signed_squared_distance);
  EXPECT_FALSE(is_vertex);
  EXPECT_LT(closest.squaredNorm(), constants::kSmallNumber);
  EXPECT_NEAR(signed_squared_distance, -1.0, constants::kSmallNumber);

  query << -1.0, 2.0;
  closest = polyline.ClosestPoint(query, &is_vertex, &segment,
                                  &signed_squared_distance);
  EXPECT_TRUE(is_vertex);
  EXPECT_TRUE(closest.isApprox(p2));
  EXPECT_NEAR(signed_squared_distance, -2.0, constants::kSmallNumber);

  query << 0.5, 2.0;
  closest = polyline.ClosestPoint(query, &is_vertex, &segment,
                                  &signed_squared_distance);
  EXPECT_FALSE(is_vertex);
  EXPECT_TRUE(closest.isApprox(Point2(0.5, 1.0)));
  EXPECT_NEAR(signed_squared_distance, -1.0, constants::kSmallNumber);

  query << 3.0, 2.0;
  closest = polyline.ClosestPoint(query, &is_vertex, &segment,
                                  &signed_squared_distance);
  EXPECT_TRUE(is_vertex);
  EXPECT_TRUE(closest.isApprox(p3));
  EXPECT_NEAR(signed_squared_distance, -2.0, constants::kSmallNumber);
}
