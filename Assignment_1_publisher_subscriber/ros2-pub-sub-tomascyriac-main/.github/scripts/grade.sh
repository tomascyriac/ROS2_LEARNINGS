#!/bin/bash

# 1. Source the ROS 2 environment
source /opt/ros/jazzy/setup.bash

# 2. Source the local workspace
if [ -f install/setup.bash ]; then
  source install/setup.bash
else
  echo "❌ Error: install/setup.bash not found. Did the build fail?"
  exit 1
fi

# 3. Run publisher in background and subscriber with timeout
echo "Starting publisher_node in the background..."
ros2 run entri_ros2_pubsub_cpp publisher_node > pub_output.log 2>&1 &
PUB_PID=$!

echo "Starting subscriber_node for 10 seconds..."
timeout 10s ros2 run entri_ros2_pubsub_cpp subscriber_node > sub_output.log 2>&1 || true

# Kill the background publisher
kill $PUB_PID 2>/dev/null || true

# 4. Check results for Subscriber output matching the correct format
if grep -Eq "Robot [0-9]+ is at [0-9.]+.* battery" sub_output.log; then
  echo "✅ Found subscriber output for robot battery status."
  exit 0
else
  echo "❌ Could not find subscriber output matching 'Robot X is at Y% battery' in log:"
  cat sub_output.log
  echo "=== Publisher Log ==="
  cat pub_output.log
  exit 1
fi
