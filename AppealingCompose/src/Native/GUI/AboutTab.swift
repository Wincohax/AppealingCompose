//
//  AboutTab.swift
//  AppealingCompose
//

import SwiftUI

struct AboutTab: View {
    let appVersion = Bundle.main.infoDictionary?["CFBundleShortVersionString"] as? String ?? "1.0.0"
    let buildNumber = Bundle.main.infoDictionary?["CFBundleVersion"] as? String ?? "1"

    var body: some View {
        ScrollView {
            VStack(spacing: 24) {
                // Xcode-style App Identity Card
                VStack(spacing: 12) {
                    ZStack {
                        Circle()
                            .fill(LinearGradient(gradient: Gradient(colors: [Color.blue, Color.cyan]), startPoint: .topLeading, endPoint: .bottomTrailing))
                            .frame(width: 96, height: 96)
                            .shadow(color: Color.black.opacity(0.15), radius: 8, x: 0, y: 4)

                        Image("ComposeIcon")
                            .resizable()
                            .aspectRatio(contentMode: .fit)
                            .frame(width: 48, height: 48)
                          //  .foregroundColor(.white)
                            .colorInvert();
                    }

                    Text("AppealingCompose")
                        .font(.system(size: 24, weight: .bold))

                    Text("Version \(appVersion) (\(buildNumber))")
                        .font(.subheadline)
                        .foregroundColor(.secondary)

                    Text("Native X11/Wayland like Compose Key Input Method for macOS")
                        .font(.caption)
                        .fontWeight(.medium)
                        .foregroundColor(.accentColor)
                        .padding(.horizontal, 12)
                        .padding(.vertical, 4)
                        .background(Color.accentColor.opacity(0.12))
                        .cornerRadius(12)
                }

                Divider()

                // Setup Guide Card
                VStack(alignment: .leading, spacing: 14) {
                    HStack(spacing: 8) {
                        Image(systemName: "gearshape.2.fill")
                            .foregroundColor(.accentColor)
                        Text("macOS Input Source Setup Instructions")
                            .font(.system(size: 15, weight: .bold))
                    }

                    VStack(alignment: .leading, spacing: 10) {
                        StepRow(number: "1", text: "Open macOS System Settings → Keyboard → Text Input.")
                        StepRow(number: "2", text: "Click 'Edit...' next to Input Sources, then click '+' to add a new source.")
                        StepRow(number: "3", text: "Select 'English' and add 'AppealingCompose' from the list.")
                        StepRow(number: "4", text: "Switch active input source to AppealingCompose from menu bar or 🌐 Fn key.")
                    }
                }
                .padding(16)
                .background(Color(NSColor.controlBackgroundColor))
                .cornerRadius(12)

                // Feature Highlights
                VStack(alignment: .leading, spacing: 12) {
                    Text("FEATURES & ARCHITECTURE")
                        .font(.caption)
                        .fontWeight(.bold)
                        .foregroundColor(.secondary)

                    GridRowView(icon: "bolt.fill", title: "C++ Composing Engine", desc: "Ultra-fast char matching using C++23 std::ranges")
                    GridRowView(icon: "doc.text.magnifyingglass", title: "Standard X11 Compose.pre", desc: "Supports thousands of standard Unicode sequences")
                    GridRowView(icon: "slider.horizontal.3", title: "Configurable Keybindings", desc: "Custom trigger keys & custom user sequence definitions")
                    GridRowView(icon: "app.badge.checkmark", title: "System Tray Native GUI", desc: "Runs in system tray, lightweight & hidden from Dock")
                }
                .padding(16)
                .background(Color(NSColor.controlBackgroundColor))
                .cornerRadius(12)

                // Footer & License
                VStack(spacing: 6) {
                    Text("Released under the GPLV3 License")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("© 2026 Wincohax. All rights reserved.")
                        .font(.caption2)
                        .foregroundColor(.secondary.opacity(0.7))
                }
                .padding(.top, 8)
            }
            .padding(24)
        }
    }
}

struct StepRow: View {
    let number: String
    let text: String

    var body: some View {
        HStack(alignment: .top, spacing: 12) {
            ZStack {
                Circle()
                    .fill(Color.accentColor)
                    .frame(width: 22, height: 22)
                Text(number)
                    .font(.caption)
                    .fontWeight(.bold)
                    .foregroundColor(.white)
            }
            Text(text)
                .font(.subheadline)
                .foregroundColor(.primary)
            Spacer()
        }
    }
}

struct GridRowView: View {
    let icon: String
    let title: String
    let desc: String

    var body: some View {
        HStack(spacing: 12) {
            Image(systemName: icon)
                .font(.system(size: 16))
                .foregroundColor(.accentColor)
                .frame(width: 24)

            VStack(alignment: .leading, spacing: 2) {
                Text(title)
                    .font(.system(size: 13, weight: .semibold))
                Text(desc)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            Spacer()
        }
    }
}
