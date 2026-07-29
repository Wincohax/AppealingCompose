<h1 align="center">
<img width="64" height="64" alt="image" src="https://github.com/user-attachments/assets/56e1ba24-b3f7-4560-8b04-1805e2268f2b" /></img>
<p style="text-align: center;">AppealingCompose</p>
</h1>

An implementation of the Compose Key (as seen in Wayland/X11) for macOS, built using the Input Method Kit (IMK).

## Status

The project is functional and can be used, though it remains a work‑in‑progress. Expect occasional issues and consider it suitable for testing and experimentation.

## Features

- Compose key support for macOS using IMK and Quartz.
- Customizable key mappings.
- Works with Wayland/X11 style compose sequences.
- System‑wide activation via input method.
- Simple configuration through the app.
- Pure C++ Backend to parse and search sequences.

## Build Instructions

1. Open the `AppealingCompose.xcodeproj` in Xcode.
2. Build the scheme and run the target.

## Usage

1. Add **AppealingCompose** to your Input Sources in **System Settings → Keyboard → Input Sources**.
2. Press the compose key (default: **Right Option**) followed by a sequence, e.g. `'` `e` → `é`.
3. Custom sequences can be added inside the App.

## Known Limitations

- Only supports US and (maybe) EU keyboard layouts out of the box.
- Hot Check only works with the first sent input and when the max possible amount of sequence chars is reached.
- Some apps might not register keyboard inputs if they were opened before AppealingCompose.
- not every X11/Wayland combination is supported (yet).

## Contributing

Contributions are welcome. Please open an issue or pull request for bugs, enhancements, or documentation updates.

## License

GPL‑3.0 – see the [LICENSE](LICENSE) file.
