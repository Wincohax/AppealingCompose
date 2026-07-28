//
//  CustomCombinationsTab.swift
//  AppealingCompose
//

import SwiftUI

struct CustomCombinationsTab: View {
    @EnvironmentObject var manager: ComposeKeyManager

    @State private var searchText: String = ""
    @State private var isAddingNew: Bool = false

    // New combination form state
    @State private var newTokensInput: String = ""
    @State private var newResultInput: String = ""
    @State private var errorMessage: String? = nil

    var filteredCustoms: [CustomCombination] {
        if searchText.isEmpty {
            return manager.customCombinations
        } else {
            return manager.customCombinations.filter {
                $0.outputChar.contains(searchText) ||
                $0.sequenceTokens.joined(separator: " ").localizedCaseInsensitiveContains(searchText)
            }
        }
    }

    var body: some View {
        VStack(spacing: 0) {
            // Header & Action Bar
            HStack(spacing: 12) {
                HStack {
                    Image(systemName: "magnifyingglass")
                        .foregroundColor(.secondary)
                    TextField("Search custom combinations...", text: $searchText)
                        .textFieldStyle(PlainTextFieldStyle())
                }
                .padding(8)
                .background(Color(NSColor.controlBackgroundColor))
                .cornerRadius(8)

                Button(action: {
                    newTokensInput = ""
                    newResultInput = ""
                    errorMessage = nil
                    isAddingNew = true
                }) {
                    HStack(spacing: 6) {
                        Image(systemName: "plus")
                        Text("Add Combination")
                    }
                    .font(.system(size: 13, weight: .semibold))
                }
                .buttonStyle(BorderedProminentButtonStyle())
            }
            .padding(16)
            .background(Color(NSColor.windowBackgroundColor))

            Divider()

            // List Content
            if manager.customCombinations.isEmpty {
                VStack(spacing: 16) {
                    Spacer()
                    Image(systemName: "character.book.closed")
                        .font(.system(size: 48))
                        .foregroundColor(.secondary.opacity(0.6))
                    Text("No Custom Combinations Yet")
                        .font(.headline)
                        .foregroundColor(.secondary)
                    Text("Click 'Add Combination' above to create custom compose key shortcuts.")
                        .font(.subheadline)
                        .foregroundColor(.secondary.opacity(0.8))
                        .multilineTextAlignment(.center)
                        .frame(maxWidth: 360)
                    Spacer()
                }
                .frame(maxWidth: .infinity, maxHeight: .infinity)
            } else {
                List {
                    ForEach(filteredCustoms) { combo in
                        HStack(spacing: 16) {
                            // Output Character Badge
                            ZStack {
                                RoundedRectangle(cornerRadius: 8)
                                    .fill(Color.accentColor.opacity(0.15))
                                    .frame(width: 44, height: 44)
                                Text(combo.outputChar)
                                    .font(.system(size: 22, weight: .bold))
                                    .foregroundColor(.accentColor)
                            }

                            // Sequence Tokens
                            VStack(alignment: .leading, spacing: 4) {
                                HStack(spacing: 6) {
                                    Text("Compose Key ⌥")
                                        .font(.system(.caption, design: .monospaced))
                                        .padding(.horizontal, 6)
                                        .padding(.vertical, 3)
                                        .background(Color.secondary.opacity(0.2))
                                        .cornerRadius(4)

                                    Text("+")
                                        .font(.caption)
                                        .foregroundColor(.secondary)

                                    ForEach(combo.sequenceTokens, id: \.self) { token in
                                        Text(token)
                                            .font(.system(.body, design: .monospaced))
                                            .fontWeight(.bold)
                                            .padding(.horizontal, 8)
                                            .padding(.vertical, 3)
                                            .background(Color.blue.opacity(0.12))
                                            .foregroundColor(.blue)
                                            .cornerRadius(6)
                                    }
                                }

                                Text("Outputs: \(combo.outputChar)")
                                    .font(.caption)
                                    .foregroundColor(.secondary)
                            }

                            Spacer()

                            // Delete Button
                            Button(action: {
                                manager.deleteCombination(combo)
                            }) {
                                Image(systemName: "trash")
                                    .foregroundColor(.red.opacity(0.8))
                            }
                            .buttonStyle(PlainButtonStyle())
                            .help("Delete custom combination")
                        }
                        .padding(.vertical, 6)
                    }
                }
                .listStyle(PlainListStyle())
            }
        }
        .sheet(isPresented: $isAddingNew) {
            VStack(alignment: .leading, spacing: 18) {
                HStack {
                    Text("Add Custom Combination")
                        .font(.headline)
                    Spacer()
                    Button("Cancel") {
                        isAddingNew = false
                    }
                    .buttonStyle(PlainButtonStyle())
                }

                Divider()

                VStack(alignment: .leading, spacing: 6) {
                    Text("Key Sequence Tokens")
                        .font(.subheadline)
                        .fontWeight(.semibold)
                    Text("Enter space-separated characters or tokens (e.g., 'a b' or '=' '=')")
                        .font(.caption)
                        .foregroundColor(.secondary)

                    TextField("e.g. = =", text: $newTokensInput)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                }

                VStack(alignment: .leading, spacing: 6) {
                    Text("Resulting Composed Character")
                        .font(.subheadline)
                        .fontWeight(.semibold)
                    Text("The output character produced when key sequence is typed")
                        .font(.caption)
                        .foregroundColor(.secondary)

                    TextField("e.g. ≡ or ã or €", text: $newResultInput)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                }

                if let err = errorMessage {
                    Text(err)
                        .font(.caption)
                        .foregroundColor(.red)
                }

                HStack {
                    Spacer()
                    Button("Save Combination") {
                        saveNewCombination()
                    }
                    .buttonStyle(BorderedProminentButtonStyle())
                }
            }
            .padding(20)
            .frame(width: 440)
        }
    }

    private func saveNewCombination() {
        let rawTokens = newTokensInput.trimmingCharacters(in: .whitespacesAndNewlines)
        let result = newResultInput.trimmingCharacters(in: .whitespacesAndNewlines)

        guard !rawTokens.isEmpty else {
            errorMessage = "Please enter sequence tokens."
            return
        }
        guard !result.isEmpty else {
            errorMessage = "Please enter an output character."
            return
        }

        let tokenList = rawTokens.components(separatedBy: .whitespaces).filter { !$0.isEmpty }
        manager.addCombination(tokens: tokenList, outputChar: result)
        isAddingNew = false
    }
}
