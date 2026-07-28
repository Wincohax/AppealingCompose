//
//  StatusBarManager.swift
//  AppealingCompose
//

import AppKit
import SwiftUI

@MainActor
@objc public class StatusBarManager: NSObject, NSMenuDelegate {
    public static let shared = StatusBarManager()

    private var statusItem: NSStatusItem?
    private var settingsWindow: NSWindow?

    public override init() {
        super.init()
    }

    public func setupStatusBar() {
        let item = NSStatusBar.system.statusItem(withLength: NSStatusItem.squareLength)
        if let button = item.button {
            if let image = NSImage(named: "ComposeIcon") {
                let resizedImage = NSImage(size: NSSize(width: 18, height: 18), flipped: false) { rect in
                    image.draw(in: rect)
                    return true
                }
                resizedImage.isTemplate = true
                button.image = resizedImage
            } else {
                button.title = "⌥C"
            }
            button.toolTip = "AppealingCompose - X11 Compose Key for macOS"
        }

        let menu = NSMenu()
        menu.delegate = self

        let headerItem = NSMenuItem(title: "AppealingCompose Active", action: nil, keyEquivalent: "")
        headerItem.isEnabled = false
        menu.addItem(headerItem)

        menu.addItem(NSMenuItem.separator())

        let settingsItem = NSMenuItem(title: "Settings...", action: #selector(openSettings), keyEquivalent: ",")
        settingsItem.target = self
        menu.addItem(settingsItem)

        let aboutItem = NSMenuItem(title: "About AppealingCompose", action: #selector(openAbout), keyEquivalent: "")
        aboutItem.target = self
        menu.addItem(aboutItem)

        menu.addItem(NSMenuItem.separator())

        let quitItem = NSMenuItem(title: "Quit AppealingCompose", action: #selector(quitApp), keyEquivalent: "q")
        quitItem.target = self
        menu.addItem(quitItem)

        item.menu = menu
        self.statusItem = item
    }

    @objc public func openSettings() {
        openSettingsWindow(selectedTab: .composeKey)
    }

    @objc public func openAbout() {
        openSettingsWindow(selectedTab: .about)
    }

    public func openSettingsWindow(selectedTab: SettingsTab = .composeKey) {
        if let window = settingsWindow {
            window.makeKeyAndOrderFront(nil)
            NSApp.activate(ignoringOtherApps: true)
            NotificationCenter.default.post(name: NSNotification.Name("SelectSettingsTab"), object: selectedTab)
            return
        }

        let settingsView = SettingsView(initialTab: selectedTab)
            .environmentObject(ComposeKeyManager.shared)

        let hostingController = NSHostingController(rootView: settingsView)
        let window = NSWindow(
            contentRect: NSRect(x: 0, y: 0, width: 680, height: 480),
            styleMask: [.titled, .closable, .miniaturizable, .resizable],
            backing: .buffered,
            defer: false
        )
        window.center()
        window.setFrameAutosaveName("AppealingComposeSettingsWindow")
        window.contentViewController = hostingController
        window.title = "AppealingCompose Settings"
        window.isReleasedWhenClosed = false

        self.settingsWindow = window
        window.makeKeyAndOrderFront(nil)
        NSApp.activate(ignoringOtherApps: true)
    }

    @objc public func quitApp() {
        NSApplication.shared.terminate(nil)
    }
}
