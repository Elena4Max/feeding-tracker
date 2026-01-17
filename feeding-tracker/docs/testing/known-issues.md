# Known Issues — Feeding Tracker

This document tracks confirmed issues discovered during testing.

---

## Issue Template

ID:
Title:
Module:
Severity: Low / Medium / High / Critical
Status: Open / In Progress / Fixed
Detected in:
Related Test Case:
Description:
Steps to Reproduce:
Expected Result:
Actual Result:
Workaround:
Notes:

---

KI-001: Logs Cleared During Active Feeding Session

Severity: Critical
Status: Open
First observed: 2026-01-16

Description

During an active feeding session (FSM state = RIGHT_ACTIVE), a message send operation was triggered and logs were cleared immediately after sending.

Observed output:

C: message sent successfully
Email sent successfully!
Logs cleared

This occurred while feeding was still in progress.

Expected Behavior

Destructive operations (log clearing, data reset) must be forbidden while FSM state = RIGHT_ACTIVE.

Sending a message must not implicitly clear logs.

Feeding session data must remain intact until the session is completed and safely persisted.

Actual Behavior

Logs were cleared during an active feeding session.

Feeding continued without historical data.

No FSM state validation prevented this operation.

Impact

Data loss of an active feeding session.

FSM continues operating on cleared state, leading to inconsistent system behavior.

Compromises reliability of feeding records.

Probable Root Cause

Missing FSM state guard before executing destructive commands.

Tight coupling between "send message" and "clear logs" logic.

Violation of separation of concerns between FSM control and data lifecycle management.

Recommended Fix

Introduce FSM state gating for all destructive operations.

Explicitly separate SEND and CLEAR commands.

Enforce the rule: No data mutation is allowed while FSM state = RIGHT_ACTIVE.

Related Test Cases

TC-BTN-01 (Single press)
