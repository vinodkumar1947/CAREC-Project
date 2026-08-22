# Hardware Reference Index

This directory contains links for the owner-controlled legacy firmware prototype. Contributors to CAREC Sim do not need these components.

## Legacy prototype references

| Component | Authoritative reference |
|---|---|
| SenseCAP Watcher W1-A | [Seeed product documentation](https://wiki.seeedstudio.com/watcher/) |
| SenseCAP Watcher hardware | [Open-source schematics](https://github.com/Seeed-Studio/OSHW-SenseCAP-Watcher) |
| ESP32-S3 | [Espressif datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf) |
| ESP32-S3 | [Technical reference manual](https://www.espressif.com/sites/default/files/documentation/esp32-s3_technical_reference_manual_en.pdf) |
| Himax WiseEye2 | [Himax product information](https://www.himax.com.tw/products/wiseeye-ai-sensing/wiseeye2-ai-processor/) |

Links may change and specifications must be verified before physical design decisions.

## Adding future references

A new physical component requires:

- a simulator or validation requirement it satisfies;
- license and redistribution review;
- power, interface, latency, and failure-mode notes;
- an architecture decision or owner-approved issue;
- confirmation that contributors are not required to purchase it.

Do not commit copyrighted vendor PDFs unless redistribution is permitted. Prefer authoritative links. If a redistributable local document is approved, use `<component>_<version>_datasheet.pdf`.
