# Vishes Seedling Fan Controller — Manual Test Plan

**Hardware required:** XIAO ESP32-S3, EMC2101 board, DS18B20 probe, 4-wire PC fan, jumper wire
**Tools required:** USB cable, Serial monitor (115200 baud), phone/tablet with Safari or Chrome, "ESP BLE Provisioning" app (Espressif)

Mark each result: `[ ]` not run · `[P]` pass · `[F]` fail · `[N/A]` not applicable

---

## 1. Boot & Serial Output

| # | Test | Expected Serial Output | Result | Notes |
|---|------|----------------------|--------|-------|
| 1.1 | Flash firmware and open Serial monitor at 115200 | No garbage characters; output begins within 4 seconds | [ ] | |
| 1.2 | Verify EMC2101 detected | `EMC2101 ready` | [ ] | |
| 1.3 | Verify DS18B20 detected | `DS18B20 sensors found: 1` | [ ] | |
| 1.4 | Verify preferences loaded | `Loaded: target=75.0F  Kp=2.00 Ki=0.100 Kd=0.000  runFull=OFF` (first boot) | [ ] | |
| 1.5 | Verify provisioning starts | `Starting BLE provisioning...` | [ ] | |

---

## 2. BLE Provisioning

| # | Test | Steps | Expected | Result | Notes |
|---|------|-------|----------|--------|-------|
| 2.1 | First-time provision | Open ESP BLE Provisioning app → scan → select `PROV_SeedlingFan` → enter PIN `9116` → select WiFi network → confirm | Board connects to WiFi; Serial shows `Connected. Open http://...` | [ ] | |
| 2.2 | IP address printed | After WiFi connects | Serial prints a valid local IP address | [ ] | |
| 2.3 | Credentials persisted after reboot | Reset board without jumper held | Board connects to WiFi without re-provisioning | [ ] | |
| 2.4 | Force re-provision | Hold jumper on D3 to GND, reset board, release after provisioning | Serial shows `Reprovision jumper held — wiping WiFi credentials`; BLE starts; board provisions fresh | [ ] | |
| 2.5 | Wrong PIN rejected | Enter incorrect PIN in provisioning app | Serial shows `Provisioning: failed — restarting`; board restarts | [ ] | |
| 2.6 | Jumper release triggers reboot | After forced re-provision completes, pull jumper | Serial shows `Jumper pulled — rebooting to normal mode`; board reboots and connects | [ ] | |

---

## 3. Status LED

| # | Test | Expected | Result | Notes |
|---|------|----------|--------|-------|
| 3.1 | LED solid during WiFi wait | Before WiFi connects, LED stays on solid | [ ] | |
| 3.2 | LED blinks when connected | After WiFi connects, LED blinks at ~1 Hz | [ ] | |
| 3.3 | LED returns to solid if WiFi drops | Disconnect router / kill AP | LED goes solid; returns to blinking when reconnected | [ ] | |

---

## 4. Temperature Probe

| # | Test | Steps | Expected | Result | Notes |
|---|------|-------|----------|--------|-------|
| 4.1 | Normal reading | Probe connected; observe Serial | `Temp: XX.X F` printed every 2 seconds; value is plausible for room temperature | [ ] | |
| 4.2 | Probe disconnect detected | Unplug DS18B20 mid-operation | Serial prints `Probe: disconnected`; dashboard Probe Status shows DANGER | [ ] | |
| 4.3 | Probe reconnect detected | Re-plug DS18B20 | Dashboard Probe Status returns to SUCCESS; temperature readings resume | [ ] | |
| 4.4 | Temperature accuracy | Place probe in ice water (~32°F) then boiling water (~212°F) | Dashboard reading within ±2°F of expected | [ ] | |

---

## 5. Fan Control

| # | Test | Steps | Expected | Result | Notes |
|---|------|-------|----------|--------|-------|
| 5.1 | Fan spins at PID output | Warm probe above target temp with hand | Fan speed increases; Serial shows increasing Fan % | [ ] | |
| 5.2 | Fan slows below target | Allow probe to cool below target | Fan % decreases toward 0 | [ ] | |
| 5.3 | RPM reading | Fan running; observe Serial | RPM value is non-zero and changes with fan speed | [ ] | |
| 5.4 | Fan off at 0% | Temp well below setpoint | Fan stops; RPM reads 0 | [ ] | |
| 5.5 | Duty cycle clamped at 100% | Force high output (warm probe far above target) | Fan % never exceeds 100 in Serial output | [ ] | |

---

## 6. Failsafe Behavior

| # | Test | Steps | Expected | Result | Notes |
|---|------|-------|----------|--------|-------|
| 6.1 | Run Full On Error = OFF, probe disconnected | Toggle `Run Full on Error` off; unplug probe | Fan stops (0%); Serial shows `Probe: disconnected` | [ ] | |
| 6.2 | Run Full On Error = ON, probe disconnected | Toggle `Run Full on Error` on; unplug probe | Fan runs at 100%; Serial shows `Probe: disconnected` | [ ] | |
| 6.3 | Recovery from failsafe | Re-plug probe after failsafe triggered | Fan resumes PID control; Probe Status returns SUCCESS | [ ] | |

---

## 7. Dashboard — Display Cards

| # | Test | Steps | Expected | Result | Notes |
|---|------|-------|----------|--------|-------|
| 7.1 | Open dashboard | Navigate to `http://<device-ip>/dash` | Page loads; all cards visible | [ ] | |
| 7.2 | Temperature card updates live | Observe card while warming probe | Temperature value updates within 2–3 seconds | [ ] | |
| 7.3 | Fan % card updates | Observe while fan speed changes | Fan % matches Serial output | [ ] | |
| 7.4 | RPM card updates | Fan running | RPM value updates and matches Serial output | [ ] | |
| 7.5 | Probe Status card | Unplug then re-plug probe | Card shows DANGER (red) when disconnected, SUCCESS (green) when connected | [ ] | |
| 7.6 | Multiple clients | Open dashboard in two browser tabs simultaneously | Both tabs update in real time | [ ] | |

---

## 8. Dashboard — Controls

| # | Test | Steps | Expected | Result | Notes |
|---|------|-------|----------|--------|-------|
| 8.1 | Target Temp slider | Move slider to 80°F | Serial prints `Target temp set to 80.0 F`; PID setpoint changes | [ ] | |
| 8.2 | Target Temp range | Move slider to minimum (60°F) and maximum (90°F) | Values clamp at 60 and 90; no crash | [ ] | |
| 8.3 | Kp slider | Adjust Kp | Serial prints `Kp set to X.XX`; fan response changes | [ ] | |
| 8.4 | Ki slider | Adjust Ki | Serial prints `Ki set to X.XXX` | [ ] | |
| 8.5 | Kd slider | Adjust Kd | Serial prints `Kd set to X.XXX` | [ ] | |
| 8.6 | Run Full On Error toggle | Toggle on then off | Serial confirms state change; dashboard reflects toggle state | [ ] | |

---

## 9. NVS Persistence

| # | Test | Steps | Expected | Result | Notes |
|---|------|-------|----------|--------|-------|
| 9.1 | Target temp survives reboot | Set target to 80°F via slider; reset board | Serial shows `Loaded: target=80.0F` on reboot | [ ] | |
| 9.2 | PID gains survive reboot | Change Kp to 3.0; reset board | Serial shows `Kp=3.00` on reboot | [ ] | |
| 9.3 | Run Full On Error survives reboot | Enable toggle; reset board | Serial shows `runFull=ON` on reboot | [ ] | |
| 9.4 | All defaults on fresh erase | Flash with `pio run --target erase`; reflash firmware | Serial shows default values (target=75°F, Kp=2.0, etc.) | [ ] | |

---

## 10. iOS Web Clip

| # | Test | Steps | Expected | Result | Notes |
|---|------|-------|----------|--------|-------|
| 10.1 | Root page loads | Navigate to `http://<device-ip>/` in Safari | Page loads without redirect | [ ] | |
| 10.2 | Add to Home Screen | Use Safari Share → Add to Home Screen | Icon appears on home screen with title "Fan Controller" | [ ] | |
| 10.3 | Opens in standalone mode | Launch from Home Screen icon | Opens fullscreen without Safari chrome/address bar | [ ] | |
| 10.4 | WebSocket stays connected | Use web clip for 5+ minutes | Dashboard continues to update; no disconnect | [ ] | |

---

## 11. Regression — Known Past Issues

These tests guard against bugs that were previously fixed.

| # | Issue | Test | Expected | Result | Notes |
|---|-------|------|----------|--------|-------|
| 11.1 | EMC2101 WHOAMI mismatch | Power cycle with EMC2101 connected | Serial shows `EMC2101 ready`, not `not found` | [ ] | Patched local lib |
| 11.2 | PWM clock order | Fan at 0% on startup; then ramp up | Fan responds immediately to PID output; no stall | [ ] | enableTachInput before configPWMClock |
| 11.3 | BLE provisioning on ESP32-S3 | Full re-provision (test 2.4) | Completes without hanging | [ ] | HANDLER_FREE_BLE required |
| 11.4 | iOS web clip redirect breaks WebSocket | Open from Home Screen, wait 30 seconds | Dashboard updates without page reload | [ ] | fetch+document.write fix |
| 11.5 | GPIO0 not used as reprovision button | Normal boot | No accidental re-provision | [ ] | GPIO0 = ROM bootloader |

---

## Test Run Log

| Date | Firmware version | Tester | Pass | Fail | Notes |
|------|-----------------|--------|------|------|-------|
| | | | | | |
