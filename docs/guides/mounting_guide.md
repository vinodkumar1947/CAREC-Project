# CAREC Mounting Guide

**Device:** SenseCAP Watcher W1-A Clear Enclosure  
**Mount:** Ball Joint (1/4" thread) + Tube Clamp (1–1.5" stainless)  
**Install time:** ~5 minutes | **Remove time:** ~2 minutes

---

## What You Need

| Item | Source | Price |
|------|--------|-------|
| SenseCAP Watcher W1-A Clear Enclosure | Seeed Studio | $59.99 |
| Ball Joint Mount (1/4" thread, 360°/±80°) | Amazon | $10–15 |
| Tube Clamp (1–1.5" adjustable, stainless steel) | Amazon | $8–12 |
| USB-C cable (1m, right-angle preferred) | Amazon | included / $5 |
| Velcro cable ties (×3) | Amazon | $3 |

**Tools:** Phillips screwdriver, adjustable spanner (optional)

---

## Step 1: Prepare the Ball Joint

1. Thread the **1/4" bolt on the ball joint** into the standard tripod mount on the **bottom of the SenseCAP Watcher**
2. Hand-tighten — finger tight is sufficient at this stage
3. Keep the locking knob loose so you can adjust angle in Step 4

---

## Step 2: Mount the Tube Clamp

1. Open the tube clamp to its maximum diameter
2. Slide the clamp around the **wheelchair armrest tube** (typically 1–1.25" diameter)
3. Position the clamp bracket **on the forward-facing side** of the armrest
4. Leave the clamp screw accessible from the top for quick removal
5. **Do not fully tighten yet** — you'll adjust position in Step 4

---

## Step 3: Attach Watcher to Clamp

1. Attach the ball joint base plate to the tube clamp bracket using the clamp's bolt
2. Seat the Watcher (on the ball joint) into the bracket
3. Route the USB-C cable toward the battery — keep cable on the **inner armrest side** away from wheels

---

## Step 4: Aim the Camera

**Target angle:** Forward-facing, tilted **~45° downward**

This captures obstacles on the floor level in front of the wheelchair.

**Test method:**
1. Power on the Watcher
2. Open SenseCraft Mate app → Live Camera view
3. Adjust ball joint tilt until the floor at ~80 cm ahead is visible in the lower 1/3 of the frame
4. Verify walls and furniture appear centred in frame
5. Lock the ball joint locking knob firmly

---

## Step 5: Cable Management

1. Attach USB-C cable to Watcher (right-angle connector preferred)
2. Run cable along the inner (body-side) face of the armrest
3. Secure with velcro ties every **~20 cm**
4. Connect to the 10,000 mAh battery — place battery in lap or pouch, not on the floor
5. Verify cable has **no slack that could reach the wheels**
6. Tug cable firmly — it should not pull free from any tie point

---

## Step 6: Final Tighten + Shake Test

1. Fully tighten the tube clamp screw
2. Fully tighten the ball joint locking knob
3. Grip the Watcher firmly and shake the armrest — **Watcher must not shift or rattle**
4. If it moves: re-tighten clamp and repeat

---

## Step 7: Functional Test

1. Power on, wait for "CAREC ready." in Serial monitor (or GREEN display)
2. Place your hand at **30 cm** → RED display + triple beep within 200 ms
3. Move hand to **80 cm** → YELLOW display + single beep
4. Remove hand → GREEN display + silence

---

## Camera Field of View

```
120° horizontal FOV (OV5647)

      ┌──────────────────────────────────────────────────────────┐
      │               120° field of view                         │
      │                                                          │
      │    ┌─────────────────────────────────────────┐          │
      │    │ Watcher camera (45° downward tilt)       │          │
      │    └─────────────────────────────────────────┘          │
      │                         ↓                               │
      │              Floor coverage at 60 cm:                   │
      │         approx 120 cm wide (full doorway width)         │
      └──────────────────────────────────────────────────────────┘
```

At 45° tilt, the camera sees the floor from ~20 cm to ~120 cm ahead.

---

## Removal (Emergency — 2 Minutes)

1. Loosen tube clamp screw (1–2 turns)
2. Lift Watcher + ball joint off the armrest bracket
3. Unplug USB-C cable from Watcher
4. Slide clamp off armrest

The wheelchair is unmodified — no drilling, no adhesive, no permanent change.

---

## Troubleshooting

| Problem | Fix |
|---------|-----|
| Watcher shifts after 30 min | Re-tighten clamp; add a rubber insert inside clamp |
| Camera faces wrong direction | Loosen ball joint, re-aim, re-lock |
| USB-C cable too short | Replace with 1.5m cable; re-route along armrest |
| Display hard to see in sunlight | Adjust ball joint to face display toward caregiver |
