/* File: stats.cpp; Mode: C++; Tab-width: 3; Author: Simon Flannery;          */

#include "stats.h"

int Stats::width = 0;
int Stats::height = 0;
unsigned long long Stats::start_time = 0;
unsigned long long Stats::finish_time = 0;
unsigned long long Stats::nonshadow_rays = 0;
unsigned long long Stats::shadow_rays = 0;
unsigned long long Stats::intersections = 0;
