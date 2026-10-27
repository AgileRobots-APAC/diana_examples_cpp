# 10 Paths and Waypoints

`09_path_and_waypoints` uses `createPath`, `addMoveJ`, `runPath`, and `destroyPath` by default to build a continuous joint path. The first waypoint is current J1 +10 degrees. When `--complex` is passed, it uses `createComplexPath`, `addMoveJByTarget`, `runComplexPath`, and `destroyComplexPath` instead, with the first waypoint at current J1 +15 degrees. The example is dry-run by default. Before real execution, check the trajectory, speed, payload, and workspace clearance.
