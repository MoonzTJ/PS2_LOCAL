# Class 19 placement and movement test

This compares the same initialized actor at the origin and at the verified
northern-warpgate asset anchor. It uses the two existing native messages:
one class-19 activation, then one location update. The location is an asset
origin; correct floor contact, deployment and playable movement are unverified.
Model 0 is preserved. It supplies no new stat, state, model or sync values.

1. Close the previous local game and server. Extract this ZIP into a fresh
   folder outside the game.
2. Run **start_profile_placement_test.cmd**, choose your copied Local client,
   select **LocalVSTest** and press **Play**.
3. At the 3D world view, close map/menus/chat input. Run
   **place_and_capture_motion.cmd once**.
4. Return to the game within **eight seconds** and remain idle. The helper
   activates class 19, captures the origin, then places the actor and waits
   fifteen seconds. Stay idle until the signal, about 25 seconds in total.
5. At **TWO beeps**, hold **W**. At **ONE beep**, release W and remain idle.
6. **THREE beeps** means finished; release any held key. Upload **local-profile-placement-….zip**
   from this helper's `reports` folder, including if the test stopped.
7. Report whether the view translated while W was held. After capture, try
   mouse-look and report rotation separately. Send a screenshot if useful.

If three beeps occur before the two-beep signal, the test stopped early;
upload the failure report. Never rerun the application command in that server
session. Each application attempt is consumed before send, including an
uncertain result; ordinary reliable transport retransmissions remain enabled.

The report includes before, activated-origin, placed-idle, placed-held and
placed-released snapshots. Each retains the original format-13 read-only
observer and 16 KiB budget. It checks actual class 19 before placement, keeps
both original packet bodies unchanged, and records loading/ground/position/
velocity/resource fields without forcing readiness. Sampled position changes
do not by themselves prove input causality, collision, safe floor or camera
attachment. No PC REA setup or separate old location command is needed.
If a native transform cache is dirty, position is reported as unavailable;
the helper records that state and continues without calling a game updater.
