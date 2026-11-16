//
//  ComposeKeyManager.swift
//  AppealingCompose
//

import Foundation
import Combine
import SwiftUI

public struct KeyOption: Identifiable, Hashable {
    public var id: Int { keyCode }
    public let name: String
    public let keyCode: Int
    public let detail: String
}

public struct CustomCombination: Identifiable, Codable, Hashable {
    public var id: String {
        return "\(sequenceTokens.joined(separator: " ")) => \(outputChar)"
    }
    public var sequenceTokens: [String]
    public var outputChar: String
    public var isCustom: Bool
}

@MainActor
@objc public class ComposeKeyManager: NSObject, ObservableObject {
    public static let shared = ComposeKeyManager()

    @Published public var selectedKeyCode: Int {
        didSet {
            UserDefaults.standard.set(selectedKeyCode, forKey: "AppealingCompose_SelectedKeyCode")
            ComposerAdapter.setComposeKeyCode(Int32(selectedKeyCode))
        }
    }

    @Published public var customCombinations: [CustomCombination] = []

    // Imported XCompose file, nil when none is loaded
    @Published public var userComposeFileName: String? = nil
    @Published public var userComposeSequenceCount: Int = 0

    public let presetKeys: [KeyOption] = [
        KeyOption(name: "Right Option / RAlt", keyCode: 0x3D, detail: "Standard Compose key for macOS (Default)"),
        KeyOption(name: "Left Option / LAlt", keyCode: 0x3A, detail: "Left Option Key"),
        KeyOption(name: "Right Command ⌘", keyCode: 0x36, detail: "Right Command key"),
        KeyOption(name: "Left Command ⌘", keyCode: 0x37, detail: "Left Command key"),
        KeyOption(name: "Right Control ⌃", keyCode: 0x3E, detail: "Right Control key"),
        KeyOption(name: "Left Control ⌃", keyCode: 0x3B, detail: "Left Control key"),
        KeyOption(name: "Caps Lock ⇪", keyCode: 0x39, detail: "Caps Lock key"),
        KeyOption(name: "Section § / Tilde ~", keyCode: 0x0A, detail: "Section / ISO Extra key"),
    ]

    private override init() {
        let savedKey = UserDefaults.standard.integer(forKey: "AppealingCompose_SelectedKeyCode")
        let initialKey = (savedKey != 0) ? savedKey : 0x3A
        self.selectedKeyCode = initialKey
        super.init()

        // Sync initial key code to C++ layer
        ComposerAdapter.setComposeKeyCode(Int32(initialKey))
        loadCustomCombinations()
        loadUserComposeFile()
    }

    public func setKey(_ keyCode: Int) {
        self.selectedKeyCode = keyCode
    }

    // MARK: - Custom Combinations Persistence

    private var supportFolder: URL {
        let fileManager = FileManager.default
        let appSupport = fileManager.urls(for: .applicationSupportDirectory, in: .userDomainMask).first!
        let folder = appSupport.appendingPathComponent("AppealingCompose", isDirectory: true)
        try? fileManager.createDirectory(at: folder, withIntermediateDirectories: true)
        return folder
    }

    private var customFilePath: URL {
        supportFolder.appendingPathComponent("CustomCompose.json")
    }

    // MARK: - User XCompose File

    // The sandbox only lets us read the picked file during the import, so we keep our own copy of it
    private var userComposeFilePath: URL {
        supportFolder.appendingPathComponent("UserXCompose")
    }

    private let userComposeNameKey = "AppealingCompose_UserComposeFileName"

    private func loadUserComposeFile() {
        let url = userComposeFilePath
        guard FileManager.default.fileExists(atPath: url.path) else { return }

        userComposeSequenceCount = ComposerAdapter.loadUserComposeFile(url.path)
        userComposeFileName = UserDefaults.standard.string(forKey: userComposeNameKey) ?? url.lastPathComponent
    }

    public func importComposeFile(from source: URL) throws {
        let isScoped = source.startAccessingSecurityScopedResource()
        defer {
            if isScoped { source.stopAccessingSecurityScopedResource() }
        }

        let data = try Data(contentsOf: source)
        try data.write(to: userComposeFilePath, options: .atomic)
        UserDefaults.standard.set(source.lastPathComponent, forKey: userComposeNameKey)
        loadUserComposeFile()
    }

    public func removeComposeFile() {
        try? FileManager.default.removeItem(at: userComposeFilePath)
        UserDefaults.standard.removeObject(forKey: userComposeNameKey)
        ComposerAdapter.clearUserComposeFile()
        userComposeFileName = nil
        userComposeSequenceCount = 0
    }

    public func loadCustomCombinations() {
        let url = customFilePath
        if FileManager.default.fileExists(atPath: url.path),
           let data = try? Data(contentsOf: url),
           let list = try? JSONDecoder().decode([CustomCombination].self, from: data) {
            self.customCombinations = list
            applyCustomCombinationsToEngine()
        }
    }

    public func saveCustomCombinations() {
        let url = customFilePath
        if let data = try? JSONEncoder().encode(customCombinations) {
            try? data.write(to: url)
        }
        applyCustomCombinationsToEngine()
    }

    public func addCombination(tokens: [String], outputChar: String) {
        let trimmedTokens = tokens.map { $0.trimmingCharacters(in: .whitespacesAndNewlines) }.filter { !$0.isEmpty }
        guard !trimmedTokens.isEmpty, !outputChar.isEmpty else { return }

        let combo = CustomCombination(sequenceTokens: trimmedTokens, outputChar: outputChar, isCustom: true)
        customCombinations.append(combo)
        saveCustomCombinations()
    }

    public func deleteCombination(at offsets: IndexSet) {
        customCombinations.remove(atOffsets: offsets)
        saveCustomCombinations()
    }

    public func deleteCombination(_ combo: CustomCombination) {
        customCombinations.removeAll { $0.id == combo.id }
        saveCustomCombinations()
    }

    private func applyCustomCombinationsToEngine() {
        // start from scratch so saved or deleted combinations don't pile up in the engine
        ComposerAdapter.clearCustomSequences()
        for combo in customCombinations {
            ComposerAdapter.addCustomSequenceTokens(combo.sequenceTokens, outputChar: combo.outputChar)
        }
    }
}
