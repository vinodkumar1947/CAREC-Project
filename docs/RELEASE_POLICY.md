# Release and Configuration Policy

## Release types

- `sim-*`: hardware-free simulation/research release.
- `fw-dev-*`: owner-only firmware development artifact; never an operational
  medical or occupied-use release.
- Physical research releases require a separate approved plan and are not
  currently authorized.

## Required evidence

Every release records source commit, toolchain/container versions, configuration,
tests, scenario report, known limitations, traceability review, dependency/SBOM
artifact and owner approval. Generated binaries are release artifacts rather
than normal source commits.

Versions follow semantic versioning within each release family. Firmware update
metadata and binaries must share exactly the same version. Releases are created
only from protected `main` and tags are owner-controlled.

## Rollback

Simulation releases roll back by selecting the previous tag. Future firmware
must provide authenticated images, anti-rollback policy, a recovery image and
confirmed healthy boot before OTA can be enabled.
