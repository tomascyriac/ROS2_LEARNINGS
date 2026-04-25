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

# 3. Run service in background and client in foreground
echo "Starting service_node in the background..."
ros2 run entri_ros2_srvcli_cpp service_node > srv_output.log 2>&1 &
SRV_PID=$!

# Give the service a moment to register on the ROS graph
sleep 3

echo "Starting client_node for 10 seconds..."
timeout 10s ros2 run entri_ros2_srvcli_cpp client_node > cli_output.log 2>&1 || true

# Kill the background service
kill $SRV_PID 2>/dev/null || true

# 4. Check results for Client output matching the expected format
if grep -Eq "Task allocation status: True.*Response: .*allocated successfully" cli_output.log; then
  echo "✅ Found client output confirming task allocation success."
  exit 0
else
  echo "❌ Could not find client output confirming success. Looking for 'Task allocation status: True' and 'allocated successfully'"
  echo "=== Client Log ==="
  cat cli_output.log
  echo "=== Service Log ==="
  cat srv_output.log
  exit 1
fi
