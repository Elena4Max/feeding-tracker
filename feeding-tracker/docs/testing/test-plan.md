# Test Plan — Feeding Tracker (ESP32 Firmware)

## 0. Purpose

This document defines the system-level test plan for the ESP32-based autonomous Feeding Tracker.

Goals:
- Validate core feeding logic correctness
- Ensure robustness against power, Wi-Fi, and time-related failures
- Verify secure handling of secrets
- Confirm autonomous operation without external dependencies
- Prepare firmware for future scaling (mobile app, backend)

---

## 1. Test Environment

- Board: ESP32 DevKit v1
- Framework: Arduino IDE
- ESP32 Core: specified in platformio.ini / Arduino Board Manager
- Storage: SD card (FAT32, 8–32 GB)
- Connectivity: Wi-Fi 2.4 GHz
- Power: USB / Battery
- Timezone: Europe/Kyiv
- Time source: NTP

---

## 2. Test Levels

### 2.1 Module / Unit Tests (Manual)
Focused validation of isolated components:
- TimeManager
- ButtonStateMachine
- SDLogger
- WiFiSender
- Config / config_secret

### 2.2 Integration Tests
Validation of interactions between modules:
- Button → Time → SD logging
- SD logging → Wi-Fi sending
- Offline queue → delayed send

### 2.3 System Tests
End-to-end scenarios on real hardware:
- Full feeding session
- Night usage
- Power loss recovery

### 2.4 Non-Functional Tests
- Stability
- Power consumption
- Security
- UX predictability

---

## 3. Configuration & Secrets (CRITICAL)

### TC-CONF-01: Missing config_secret.h
**Steps:**
1. Remove `config/config_secret.h`
2. Compile firmware

**Expected:**
- Compilation fails
- Error message clearly indicates missing secrets
- Firmware cannot be built without explicit configuration

---

### TC-CONF-02: Secrets excluded from git
**Steps:**
1. Run `git status`
2. Inspect staged files

**Expected:**
- `config_secret.h` is NOT tracked
- `config_secret.example.h` IS tracked

---

### TC-CONF-03: Secrets are not logged
**Steps:**
1. Enable Serial logging
2. Trigger email sending
3. Inspect Serial output and SD logs

**Expected:**
- No plaintext secrets in logs
- Allowed: masked or generic messages (e.g. "email enabled")

---

## 4. TimeManager

### TC-TIME-01: Initial time synchronization
**Steps:**
1. Flash clean firmware
2. Enable Wi-Fi
3. Boot device

**Expected:**
- Time synchronized via NTP
- Human-readable timestamps
- Correct timezone (Europe/Kyiv)

---

### TC-TIME-02: Reboot without internet
**Steps:**
1. Synchronize time
2. Disable Wi-Fi
3. Reboot ESP32

**Expected:**
- Last known time is preserved
- No fallback to 1970
- Logs remain consistent

---

### TC-TIME-03: Midnight boundary
**Steps:**
1. Start feeding session at 23:59
2. Finish after 00:00

**Expected:**
- Date rollover handled correctly
- Duration is valid
- No negative timestamps

---

## 5. ButtonStateMachine

### FSM Expected States
IDLE → FEEDING → COMPLETED  
Optional: CANCELLED

---

### TC-BTN-01: Single press
**Steps:**
1. Press button once
2. Wait

**Expected:**
- Feeding session starts
- Single log entry
- No debounce artifacts

---

### TC-BTN-02: Long press
**Steps:**
1. Hold button > N seconds

**Expected:**
- Defined behavior triggered (cancel / ignore)
- No accidental start/stop

---

### TC-BTN-03: Rapid presses
**Steps:**
1. Perform 5–10 fast clicks

**Expected:**
- Single valid state transition
- FSM remains stable
- No freezes

---

## 6. SDLogger

### TC-SD-01: Log write
**Steps:**
1. Perform one feeding session
2. Inspect SD card

**Expected:**
- Log file created
- Valid JSON / CSV format
- Readable timestamps

---

### TC-SD-02: SD card missing
**Steps:**
1. Remove SD card
2. Boot device
3. Start feeding

**Expected:**
- Device does not crash
- Data buffered or queued
- Clear error indication

---

### TC-SD-03: SD full
**Steps:**
1. Fill SD card
2. Attempt logging

**Expected:**
- Graceful error handling
- No data corruption
- No loss of existing logs

---

## 7. WiFiSender / Email

### TC-MAIL-01: Successful send
**Steps:**
1. Enable Wi-Fi
2. Complete feeding session
3. Wait for send

**Expected:**
- Email received
- Correct session data
- No duplicates

---

### TC-MAIL-02: Wi-Fi unavailable
**Steps:**
1. Disable Wi-Fi
2. Complete feeding session

**Expected:**
- Data stored locally
- Send deferred
- Main loop remains responsive

---

### TC-MAIL-03: Invalid credentials
**Steps:**
1. Set wrong EMAIL_PASSWORD
2. Trigger send

**Expected:**
- Explicit error reported
- Retry attempts limited
- Device continues operating

---

## 8. System Scenarios

### TC-SYS-01: Full feeding flow
**Scenario:**
Start → feed → stop → log → email

**Expected:**
- Fully automatic flow
- No manual intervention

---

### TC-SYS-02: Power loss during feeding
**Steps:**
1. Start feeding
2. Cut power
3. Restore power

**Expected:**
- Session closed or marked incomplete
- No undefined states
- Data not lost

---

### TC-SYS-03: Night usage
**Steps:**
1. Use device in low-light environment

**Expected:**
- Minimal light/noise
- Predictable behavior
- No distracting signals

---

## 9. Non-Functional Tests

### Power
- Deep sleep enabled
- No constant Wi-Fi polling
- Battery life meets expectations

### Stability
- 24h continuous operation
- No memory leaks
- No watchdog resets

### Security
- No secrets in logs
- No secrets in repository
- No hardcoded tokens

---

## 10. Release Checklist

- [ ] Firmware builds without warnings
- [ ] Secrets excluded from git
- [ ] Operates without Wi-Fi
- [ ] Operates without SD card
- [ ] Time handling correct
- [ ] No data loss detected

