/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PlatooAudioProcessorEditor::PlatooAudioProcessorEditor (PlatooAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), scale("C", "Major")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    audioProcessor.setScale(scale);
    setSize (minWidth, minHeight);
    setResizable(true, true);
    setResizeLimits (minWidth, minHeight, maxWidth, maxHeight);
    
    addAndMakeVisible(keyDropdown);
    addAndMakeVisible(scaleDropdown);
    addAndMakeVisible(scaleSectionLabel);
    addAndMakeVisible(scaleSectionKeyLabel);
    addAndMakeVisible(scaleSectionScaleLabel);
//    addAndMakeVisible(octDown);
    
    scaleSectionLabel.setFont (juce::Font (16.0f, juce::Font::bold));
    scaleSectionLabel.setText("Scales", juce::dontSendNotification);
    scaleSectionLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    scaleSectionKeyLabel.setText("Key", juce::dontSendNotification);
    scaleSectionKeyLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    scaleSectionScaleLabel.setText("Scale", juce::dontSendNotification);
    scaleSectionScaleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    
    setUpDropdown(keyDropdown, platoo::Note::notes, 1, true);
    setUpDropdown(scaleDropdown, platoo::Scale::scalesNames, 1, true);

    octDown.setTitle("Oct Down");
    octDown.setButtonText("Oct Down");
    drawScaleSectionPiano(10, 90);
    
    itor[1] = keyC;
    itor[2] = keyDb;
    itor[3] = keyD;
    itor[4] = keyEb;
    itor[5] = keyE;
    itor[6] = keyF;
    itor[7] = keyGb;
    itor[8] = keyG;
    itor[9] = keyAb;
    itor[10] = keyA;
    itor[11] = keyBb;
    itor[12] = keyB;
    
    juce::Timer::startTimer(1);
}

PlatooAudioProcessorEditor::~PlatooAudioProcessorEditor()
{
    juce::Timer::stopTimer();
}

//==============================================================================
void PlatooAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(backgroundColour);
    
    g.setColour(juce::Colour(100, 90, 85));
    g.fillRect(scaleSectionPianoTopBorder);
    g.fillRect(scaleSectionPianoBottomBorder);
    g.fillRect(scaleSectionPianoLeftBorder);
    g.fillRect(scaleSectionPianoRightBorder);
    for (auto note : scale.getNotes()) {
        if (note.getIsNatural()) {
            if (note.getInScale()) {
                g.setColour(notesColours["naturalActive"]);
                g.fillRect(itor[note.getId()]);
            } else {
                g.setColour(notesColours["naturalNotActive"]);
                g.fillRect(itor[note.getId()]);
            }
            
            if (note.getId() == currentNoteNumber) {
                g.setColour(juce::Colours::orange);
                g.fillRect(itor[note.getId()]);
            }
        }
    }
    
    // sharps and flats have to be painted after the naturals because order of painting
    // matters in juce. this way shaprs/flat are drawn correctly on top of naturals
    for (auto note : scale.getNotes()) {
        if (!note.getIsNatural()) {
            if (note.getInScale()) {
                g.setColour(notesColours["sharpActive"]);
                g.fillRect(itor[note.getId()]);
            } else {
                g.setColour(notesColours["sharpNotActive"]);
                g.fillRect(itor[note.getId()]);
            }
            
            if (note.getId() == currentNoteNumber) {
                g.setColour(juce::Colours::orange);
                g.fillRect(itor[note.getId()]);
            }
        }
    }
}

void PlatooAudioProcessorEditor::resized()
{

    scaleSectionLabel.setBounds(10, 10, 50, 15);
    scaleSectionKeyLabel.setBounds(10, 30, 40, 15);
    keyDropdown.setBounds(10, 50, 60, 30);
    scaleSectionScaleLabel.setBounds(90, 30, 100, 15);
    scaleDropdown.setBounds(90, 50, 100, 30);
}

void PlatooAudioProcessorEditor::comboBoxChanged(juce::ComboBox *box) {
//    std::cout << keyDropdown.getText() << " | " << scaleDropdown.getText() << std::endl;
    if (box == &keyDropdown || box == &scaleDropdown) {
        platoo::Scale newScale(keyDropdown.getText().toStdString(),
                       scaleDropdown.getText().toStdString());
        scale = newScale;
        audioProcessor.setScale(newScale);
        repaint();
    }
}

void PlatooAudioProcessorEditor::setUpDropdown(juce::ComboBox &dropdown, const std::vector<std::string> &options, int selectedOption, bool addListener) {

    for (int i = 0; i < options.size(); i++) {
        dropdown.addItem(options[i], i+1);
    }

    dropdown.setEditableText(false);
    dropdown.setSelectedId(selectedOption);

    if (addListener)
        dropdown.addListener(this);
}

void PlatooAudioProcessorEditor::timerCallback() {
    currentNoteNumber = audioProcessor.getCurrentNoteNumber();
    if (currentNoteNumber > 0) {
        currentNoteNumber = currentNoteNumber % 12 + 1;
        repaint();
    }
}

void PlatooAudioProcessorEditor::drawScaleSectionPiano(int x, int y) {
    int numOfNaturalNotes = 7;
    int naturalNotePadding = 1; // space between natural notes
    int allNaturalNotesPadding = (numOfNaturalNotes-1) * naturalNotePadding;
    int borderSize = 3;
    int scalePianoY = y+borderSize;
    int scalePianoX = x+borderSize;
    int scalePianoNaturalsHeight = 55;
    int scalePianoFlatsHeight = 25;
    int scalePianoNaturalsWidth = 25;
    int scalePianoFlatsWidth = 18;
    
    int bottomBorderY = scalePianoY + scalePianoNaturalsHeight;
    int leftRightBorderHeight = scalePianoNaturalsHeight + 2 * borderSize;
    int rightBorderX = x + borderSize + numOfNaturalNotes * scalePianoNaturalsWidth + allNaturalNotesPadding;
    int topBottomBorderWidth = 2 * borderSize + numOfNaturalNotes * scalePianoNaturalsWidth + allNaturalNotesPadding;
    
    scaleSectionPianoTopBorder = juce::Rectangle<int>(x, y, topBottomBorderWidth, borderSize);
    scaleSectionPianoBottomBorder = juce::Rectangle<int>(x, bottomBorderY, topBottomBorderWidth, borderSize);
    scaleSectionPianoLeftBorder = juce::Rectangle<int>(x, y, borderSize, leftRightBorderHeight);
    scaleSectionPianoRightBorder = juce::Rectangle<int>(rightBorderX, y, borderSize, leftRightBorderHeight);
    // Scale piano roll
    
    keyC = juce::Rectangle<int>(scalePianoX, scalePianoY, scalePianoNaturalsWidth, scalePianoNaturalsHeight);
    keyDb = juce::Rectangle<int>(keyC.getX()+keyC.getWidth() - scalePianoFlatsWidth / 2, scalePianoY, scalePianoFlatsWidth, scalePianoFlatsHeight);
    keyD = juce::Rectangle<int>(scalePianoX+keyC.getWidth()+naturalNotePadding, scalePianoY, scalePianoNaturalsWidth, scalePianoNaturalsHeight);
    keyEb = juce::Rectangle<int>(keyD.getX()+keyD.getWidth() - scalePianoFlatsWidth / 2, scalePianoY, scalePianoFlatsWidth, scalePianoFlatsHeight);
    keyE = juce::Rectangle<int>(keyD.getX()+keyD.getWidth()+naturalNotePadding, scalePianoY, scalePianoNaturalsWidth, scalePianoNaturalsHeight);
    keyF = juce::Rectangle<int>(keyE.getX()+keyE.getWidth()+naturalNotePadding, scalePianoY, scalePianoNaturalsWidth, scalePianoNaturalsHeight);
    keyGb = juce::Rectangle<int>(keyF.getX()+keyF.getWidth() - scalePianoFlatsWidth / 2, scalePianoY, scalePianoFlatsWidth, scalePianoFlatsHeight);
    keyG = juce::Rectangle<int>(keyF.getX()+keyF.getWidth()+naturalNotePadding, scalePianoY, scalePianoNaturalsWidth, scalePianoNaturalsHeight);
    keyAb = juce::Rectangle<int>(keyG.getX()+keyG.getWidth() - scalePianoFlatsWidth / 2, scalePianoY, scalePianoFlatsWidth, scalePianoFlatsHeight);
    keyA = juce::Rectangle<int>(keyG.getX()+keyG.getWidth()+naturalNotePadding, scalePianoY, scalePianoNaturalsWidth, scalePianoNaturalsHeight);
    keyBb = juce::Rectangle<int>(keyA.getX()+keyA.getWidth() - scalePianoFlatsWidth / 2, scalePianoY, scalePianoFlatsWidth, scalePianoFlatsHeight);
    keyB = juce::Rectangle<int>(keyA.getX()+keyA.getWidth()+naturalNotePadding, scalePianoY, scalePianoNaturalsWidth, scalePianoNaturalsHeight);
}
