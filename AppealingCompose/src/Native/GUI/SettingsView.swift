//
//  SettingsView.swift
//  AppealingCompose
//

import SwiftUI

public enum SettingsTab: String, CaseIterable, Identifiable {
    case composeKey = "Compose Key"
    case customCombinations = "Custom Combinations"
    case about = "About"

    public var id: String { rawValue }

    var iconName: String {
        switch self {
        case .composeKey: return "keyboard"
        case .customCombinations: return "plus.square.on.square"
        case .about: return "info.circle"
        }
    }
}

public struct SettingsView: View {
    @EnvironmentObject var manager: ComposeKeyManager
    @State private var selectedTab: SettingsTab

    public init(initialTab: SettingsTab = .composeKey) {
        _selectedTab = State(initialValue: initialTab)
    }

    public var body: some View {
        VStack(spacing: 0) {
            // Xcode-Style Preferences Toolbar
            HStack(spacing: 4) {
                Spacer()
                ForEach(SettingsTab.allCases) { tab in
                    Button(action: {
                        withAnimation(.easeInOut(duration: 0.15)) {
                            selectedTab = tab
                        }
                    }) {
                        VStack(spacing: 4) {
                            Image(systemName: tab.iconName)
                                .font(.system(size: 18, weight: .medium))
                            Text(tab.rawValue)
                                .font(.system(size: 11, weight: selectedTab == tab ? .semibold : .regular))
                        }
                        .foregroundColor(selectedTab == tab ? .accentColor : .secondary)
                        .padding(.horizontal, 16)
                        .padding(.vertical, 8)
                        .background(selectedTab == tab ? Color.accentColor.opacity(0.12) : Color.clear)
                        .cornerRadius(8)
                    }
                    .buttonStyle(PlainButtonStyle())
                }
                Spacer()
            }
            .padding(.vertical, 8)
            .background(Color(NSColor.windowBackgroundColor))

            Divider()

            // Active Tab Content
            Group {
                switch selectedTab {
                case .composeKey:
                    ComposeKeyTab()
                case .customCombinations:
                    CustomCombinationsTab()
                case .about:
                    AboutTab()
                }
            }
            .frame(maxWidth: .infinity, maxHeight: .infinity)
        }
        .frame(minWidth: 640, minHeight: 460)
        .onReceive(NotificationCenter.default.publisher(for: NSNotification.Name("SelectSettingsTab"))) { note in
            if let tab = note.object as? SettingsTab {
                selectedTab = tab
            }
        }
    }
}
