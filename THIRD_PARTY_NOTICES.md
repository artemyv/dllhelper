# Third-Party Notices

This repository includes or references third-party components.

## Code
- bblanchon/dllhelper — MIT — https://github.com/bblanchon/dllhelper
- fginez/dllhelper — MIT — https://github.com/fginez/dllhelper
- bemanproject/exemplar — Apache-2.0 — CMake snippets — https://github.com/bemanproject/exemplar

## GitHub Actions and Reusable Workflows (referenced)
These are referenced in workflow YAML; licenses are governed by their upstream repositories.

- actions/checkout@v4 — https://github.com/actions/checkout
- jwlawson/actions-setup-cmake@802fa1a2c4e212495c05bf94dba2704a92a472be (v2.0.2) — https://github.com/jwlawson/actions-setup-cmake
- SonarSource/sonarqube-scan-action@a31c9398be7ace6bbfaf30c0bd5d415f843d45e9 (v7.0.0) — https://github.com/SonarSource/sonarqube-scan-action
- bemanproject/infra-workflows reusable workflows @1.2.1 — https://github.com/bemanproject/infra-workflows
  - .github/workflows/reusable-beman-preset-test.yml
  - .github/workflows/reusable-beman-build-and-test.yml
  - .github/workflows/reusable-beman-pre-commit.yml

Notes:
- Actions are referenced, not vendored. If any action code is copied into this repo in the future, include its LICENSE alongside the copied files.
- Keep versions pinned for supply-chain safety and reproducibility.
