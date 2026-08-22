# Security Policy

## Supported scope

Only the latest `main` revision and explicitly published releases receive
security fixes. There is currently no operational or occupied-use release.

## Reporting

Do not open a public issue for a vulnerability that could affect command
control, firmware authenticity, credentials, personal information or a future
physical prototype. Use GitHub's private vulnerability reporting for this
repository. If that feature is unavailable, contact the repository owner
privately through the verified GitHub profile and disclose only the minimum
information needed to establish a secure channel.

Include affected revision, impact, reproduction conditions and suggested
mitigation. Never include real credentials, participant data or instructions
that could put a wheelchair user at risk.

## Current security boundary

- Network OTA is disabled by default.
- BLE is telemetry-only and must not carry safety commands.
- Community code must not access physical wheelchair controls.
- Secrets and identifiable health/participant data are prohibited in Git.
- Security findings do not authorize testing on an occupied wheelchair.
