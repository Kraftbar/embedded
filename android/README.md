# Android and watch experiments

Source recovered from LOQ on 2026-09-09, without application changes.

- `android-docker-dev/`: Docker image, Compose setup and CLI scripts for Android
  builds, ADB and watch-battery measurements.
- `projects/wff-watchface/`: Watch Face Format resource project.
- `projects/pure-c-watchface/`: native renderer with a Java watch-face service.
- `projects/hello-c-pure/` and `projects/hello-c/`: native/Java watch experiments.

Existing live paths on LOQ remain `~/android-docker-dev` and
`~/android-projects`. Some scripts/docs refer to those paths; importing source
here does not migrate that installation or reconnect a phone/watch.

The PHP interfaces under the original tooling's `www/` and the live web root
are intentionally outside this import. Website tracking and symlink/deployment
choices remain separate, as requested. The CLI tooling is preserved here;
the existing upstream README still describes its original web GUI as context.

Build caches, APKs, measurements and local signing/ADB keys are not source.
They remain on LOQ. No Android image/APK rebuild or device installation was
performed during recovery. Validation checked shell syntax, XML parsing and
byte-for-byte source preservation.
