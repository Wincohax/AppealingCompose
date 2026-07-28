//
//  AppDelegateGUI.swift
//  AppealingCompose
//

import AppKit

@MainActor
@objc public class AppDelegateGUI: NSObject {
    @objc public static func initializeGUI() {
        StatusBarManager.shared.setupStatusBar()
        _ = ComposeKeyManager.shared
    }
}
