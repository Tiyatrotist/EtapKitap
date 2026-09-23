# Packaging EtapKitap for Pardus ETAP

EtapKitap provides ready-to-use Debian package definitions and standard desktop integration assets for Pardus ETAP 23.

---

## 1. Debian Package (`.deb`) Build

To generate an official `.deb` package on Pardus ETAP or Debian systems:

### Requirements

```bash
sudo apt-get install -y debhelper devscripts build-essential
```

### Build Steps

From the root of the repository:

```bash
# Build the binary package
dpkg-buildpackage -us -uc -b

# Install the generated package
sudo dpkg -i ../etapkitap_1.0.0-1_amd64.deb

# Fix missing dependencies if needed
sudo apt-get install -f
```

---

## 2. Desktop & MIME Integration

The package installs standard desktop files under `/usr/share/`:

- **Application Launcher**: `/usr/share/applications/etapkitap.desktop`
  - Registered categories: `Education`, `Viewer`, `Qt`.
  - Supports file drag-and-drop and standard file manager associations.
- **MIME Type Definition**: `/usr/share/mime/packages/x-akbook.xml`
  - Registers `application/x-akbook` with glob pattern `*.akbook`.
- **AppStream Metainfo**: `/usr/share/metainfo/etapkitap.metainfo.xml`
  - Complies with AppStream standards for software centers.

---

## 3. Post-Installation Verification

Verify file association on Pardus ETAP:

```bash
update-mime-database /usr/share/mime
update-desktop-database /usr/share/applications

# Check association
xdg-mime query default application/x-akbook
# Should output: etapkitap.desktop
```
