//
//  ComposeKeyTab.swift
//  AppealingCompose
//

import SwiftUI

struct ComposeKeyTab: View {
    @EnvironmentObject var manager: ComposeKeyManager
    @State private var customKeyCodeInput: String = ""
    @State private var isCustomMode: Bool = false
    @State private var showSavedToast: Bool = false

    var currentOptionName: String {
        if let match = manager.presetKeys.first(where: { $0.keyCode == manager.selectedKeyCode }) {
            return match.name
        }
        return "Custom Keycode (0x\(String(manager.selectedKeyCode, radix: 16, uppercase: true)))"
    }

    var body: some View {
        ScrollView {
            VStack(alignment: .leading, spacing: 20) {
                // Header Banner
                HStack(spacing: 16) {
                    ZStack {
                        RoundedRectangle(cornerRadius: 14, style: .continuous)
                            .fill(LinearGradient(gradient: Gradient(colors: [Color.blue, Color.purple]), startPoint: .topLeading, endPoint: .bottomTrailing))
                            .frame(width: 56, height: 56)
                        
                        Image("ComposeIcon")
                            .resizable()
                            .aspectRatio(contentMode: .fit)
                            .frame(width: 36, height: 36)
                          //  .foregroundColor(.white)
                            .colorInvert();
                    }

                    VStack(alignment: .leading, spacing: 4) {
                        Text("Compose Key Selection")
                            .font(.system(size: 18, weight: .bold))
                        Text("Choose which modifier or key triggers character composition.")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                    }
                }
                .padding()
                .background(Color(NSColor.controlBackgroundColor))
                .cornerRadius(12)

                // Key Picker Section
                VStack(alignment: .leading, spacing: 14) {
                    Text("TRIGGER KEY")
                        .font(.caption)
                        .fontWeight(.bold)
                        .foregroundColor(.secondary)

                    VStack(spacing: 8) {
                        ForEach(manager.presetKeys) { option in
                            Button(action: {
                                isCustomMode = false
                                manager.setKey(option.keyCode)
                                triggerToast()
                            }) {
                                HStack {
                                    VStack(alignment: .leading, spacing: 2) {
                                        Text(option.name)
                                            .font(.system(size: 14, weight: .semibold))
                                            .foregroundColor(.primary)
                                        Text(option.detail)
                                            .font(.caption)
                                            .foregroundColor(.secondary)
                                    }

                                    Spacer()

                                    Text("0x\(String(option.keyCode, radix: 16, uppercase: true))")
                                        .font(.system(.caption, design: .monospaced))
                                        .padding(.horizontal, 8)
                                        .padding(.vertical, 4)
                                        .background(Color.secondary.opacity(0.15))
                                        .cornerRadius(6)

                                    if manager.selectedKeyCode == option.keyCode && !isCustomMode {
                                        Image(systemName: "checkmark.circle.fill")
                                            .foregroundColor(.accentColor)
                                            .font(.system(size: 18))
                                    } else {
                                        Image(systemName: "circle")
                                            .foregroundColor(.secondary.opacity(0.4))
                                            .font(.system(size: 18))
                                    }
                                }
                                .padding(12)
                                .background(manager.selectedKeyCode == option.keyCode && !isCustomMode ? Color.accentColor.opacity(0.1) : Color(NSColor.controlBackgroundColor))
                                .cornerRadius(10)
                                .overlay(
                                    RoundedRectangle(cornerRadius: 10)
                                        .stroke(manager.selectedKeyCode == option.keyCode && !isCustomMode ? Color.accentColor.opacity(0.4) : Color.clear, lineWidth: 1.5)
                                )
                            }
                            .buttonStyle(PlainButtonStyle())
                        }
                    }

                    Divider()
                        .padding(.vertical, 4)

                    // Custom Key Code Input
                    HStack {
                        VStack(alignment: .leading, spacing: 2) {
                            Text("Custom macOS Keycode")
                                .font(.system(size: 14, weight: .semibold))
                            Text("Enter custom CGVirtualKeyCode (in Hex 0x.. or Decimal)")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }

                        Spacer()

                        TextField("e.g. 0x3A or 58", text: $customKeyCodeInput)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                            .frame(width: 130)

                        Button("Apply") {
                            applyCustomKeyCode()
                        }
                        .buttonStyle(BorderedProminentButtonStyle())
                    }
                    .padding(12)
                    .background(Color(NSColor.controlBackgroundColor))
                    .cornerRadius(10)
                }

                // Active Status Card
                HStack {
                    Image(systemName: "checkmark.shield.fill")
                        .font(.title2)
                        .foregroundColor(.green)
                    
                    VStack(alignment: .leading, spacing: 2) {
                        Text("Active Compose Key: \(currentOptionName)")
                            .font(.system(size: 14, weight: .bold))
                        Text("Pressing this key triggers text composition sequence listener.")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    Spacer()
                }
                .padding()
                .background(Color.green.opacity(0.1))
                .cornerRadius(10)

                if showSavedToast {
                    HStack {
                        Image(systemName: "checkmark")
                        Text("Compose Key Updated!")
                    }
                    .font(.caption)
                    .fontWeight(.bold)
                    .foregroundColor(.white)
                    .padding(.horizontal, 16)
                    .padding(.vertical, 8)
                    .background(Color.accentColor)
                    .cornerRadius(20)
                    .transition(.move(edge: .bottom).combined(with: .opacity))
                }
            }
            .padding(20)
        }
    }

    private func applyCustomKeyCode() {
        let text = customKeyCodeInput.trimmingCharacters(in: .whitespacesAndNewlines)
        var code: Int?
        if text.hasPrefix("0x") || text.hasPrefix("0X") {
            let hexStr = text.dropFirst(2)
            code = Int(hexStr, radix: 16)
        } else {
            code = Int(text)
        }

        if let validCode = code, validCode > 0 {
            isCustomMode = true
            manager.setKey(validCode)
            triggerToast()
        }
    }

    private func triggerToast() {
        withAnimation {
            showSavedToast = true
        }
        DispatchQueue.main.asyncAfter(deadline: .now() + 2) {
            withAnimation {
                showSavedToast = false
            }
        }
    }
}
