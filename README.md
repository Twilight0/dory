<p align="center">
  <img src="dory-icon.jpg" alt="Dory Icon" width="160" height="160" style="border-radius: 20%; box-shadow: 0 4px 10px rgba(0,0,0,0.35);">
</p>

<h1 align="center">Dory</h1>

<p align="center">
  <strong>A premium, standalone file chooser portal backend and file manager</strong>
</p>

<p align="center">
  <a href="https://github.com/Twilight0/dory/actions/workflows/build.yml">
    <img src="https://github.com/Twilight0/dory/actions/workflows/build.yml/badge.svg" alt="Build Status">
  </a>
  <img src="https://img.shields.io/badge/License-GPL%203.0-blue.svg" alt="License: GPL 3.0">
  <img src="https://img.shields.io/badge/Platform-Cinnamon%20%7C%20Linux-teal.svg" alt="Platform: Cinnamon & Linux">
</p>

---

## 🐟 Overview

**Dory** is a specialized fork of the Nemo file manager, specifically tailored to serve as a high-fidelity, native **D-Bus File Chooser Portal helper**. 

Unlike standard file managers, **Dory** is fully decoupled and optimized to run alongside your favorite desktop components (even standard Nemo) with a **Zero Conflict Footprint**. It enables rich, native folder and file selection dialogs for flatpaks, sandboxed apps, and any desktop clients utilizing the `xdg-desktop-portal-filepicker` backend.

---

## ✨ Features & Enhancements

Dory preserves the power of the Nemo engine while introducing modern custom features for portal dialog orchestration:

### 🛡️ Standalone & Zero Conflict
*   **Complete Namespacing**: Every binary, desktop file, D-Bus service, namespace, library, and GSettings schema has been renamed from `nemo` to `dory`.
*   **Side-by-Side Coexistence**: Can be installed concurrently with official Nemo packages without overlapping files, config schemas, or D-Bus activations.

### 🎨 Polished Desktop & Portal Integration
*   **Premium Brand Branding**: Features custom Blue Tang fish application icons and matching visual tokens.
*   **Dual View support**: Seamlessly renders sidebars, compact view grids, and list views inside the portal dialogue frame.
*   **Slider-based Image Previews**: Real-time thumbnail scaling/zooming is fully supported inside the dialog.

### 🔧 Custom Portal Overrides
*   **Localized "Save" Override**: Automatically changes the action button label to localized `_Save` using gettext translations when invoked in save dialog mode.
*   **Intelligent Overwrite Modal**: Intercepts save paths; if a file already exists, it displays a standard modal confirmation dialog asking the user if they wish to overwrite, keeping the picker open if they select "No".
*   **GApplication Lifecycle Daemon**: Restructured D-Bus service activation handles daemon lifespans via GApplication holds, preventing the background process from terminating while portal selections are pending.

---

## 🛠️ Compilation & Installation

Dory is built using the Meson build system.

### Dependencies
Ensure the following library packages (or their distribution equivalents) are installed:
*   `cinnamon-desktop`
*   `libexif`
*   `exempi`
*   `xapp`
*   `gtk3`
*   `gobject-introspection`

### Build Command
```bash
meson setup build \
  --prefix=/usr \
  --buildtype=release \
  -Ddeprecated_warnings=false \
  -Dempty_view=false \
  -Dexif=true \
  -Dgtk_doc=false \
  -Dgtk_layer_shell=false \
  -Dselinux=false \
  -Dtracker=false \
  -Dxmp=true

meson compile -C build
sudo meson install -C build
```

---

## 🚀 Running as a Portal Backend
To use Dory for file picking, pair it with the **`xdg-desktop-portal-filepicker`** backend implementation. When flatpaks or portal-compliant apps query file selection, they will automatically spawn the Dory engine under the hood.

---

## 📜 License
Dory is free software licensed under the **GPL-3.0-or-later** and **LGPL-2.1-or-later**.
