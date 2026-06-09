# 🤝 Contributing Guide - Zappy Project

## 🌿 1. Branching Strategy (Git Workflow)

We **never** push code directly to the `main` branch. Every new task must have its own dedicated branch, following this naming convention:

* **`feat/23-feature-name`**: For adding a new core feature (e.g., `feat/23-server-elevation`).
* **`fix/54-bug-name`**: For bug fixes (e.g., `fix/54-gui-segfault-disconnect`).
* **`chore/37-task-name`**: For configuration, CI/CD, or tooling updates (e.g., `chore/37-setup-pre-commit`).
* **`docs/87-topic-name`**: For adding or updating documentation (e.g., `docs/87-architecture-map`).

## 🛠 2. Pre-commit & Formatting (Mandatory)

To avoid action failure, we use **pre-commit** to automatically enforce our coding standards before every commit (Clang-format for C++, Ruff for Python).

**To do immediately after cloning the repository:**

```bash
pip install pre-commit
pre-commit install
pre-commit install --hook-type commit-msg
```

## 📝 3. Commit Convention

We strictly follow the **Conventional Commits** standard. The pre-commit linter will block any improperly formatted commit.

**Expected format:** `type(optional-scope): description in english`

**Valid Examples:**

* `feat(server): implement poll multiplexing`
* `fix(gui): resolve player rendering overlap`
* `test(ai): add unit tests for survival state`
* `chore(ci): update github actions cache`

## 👀 4. Pull Requests & Code Reviews

Code quality is everyone's responsibility. A Pull Request (PR) can only be merged if it meets the following validation rules:

* **Base Rule (2 Approvals):** Any PR must be approved by **at least 2 people**: the other person working on the same topic (Server or GUI) + 1 person from another sub-group (to prevent knowledge silos).
* **AI Edge Case (Solo Developer):** PRs regarding the AI require the approval of the two other people who have knowledge of the topic.
* **Core / Critical Features:** Major modifications (e.g., rewriting the server's main loop or changing the network protocol) require a **global review from the entire team**.

## 🧪 5. Testing & Quality Assurance (QA)

* **Unit Tests (UT):** A minimum test coverage is required to validate a PR touching critical business logic (parsing, elevation, resource management).
* **Load Testing (Server):** Since the server must handle non-blocking multiplexing with `poll`, the Server team commits to validating their architecture using load-testing scripts (simulating dozens of simultaneous connections, malformed requests, and abrupt disconnections).

## 📋 6. Log Standardization

To facilitate cross-debugging between the Server, the GUI, and the AI, **all binaries** must print their logs to the terminal using the following strict format:

`[BINARY] [SEVERITY] Message`

*(Note: Adding a `[TIMESTAMP]` at the beginning of the log is **optional** but highly recommended for local debugging of synchronization issues).*

**Examples:**

* `[SERVER] [INFO] Connection accepted on socket 4`
* `[14:32:01.110] [AI_TEAM1] [DEBUG] Sending command: Forward`
* `[GUI] [ERROR] Failed to parse player inventory`
