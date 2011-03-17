#include "pad_highlighter.h"

void BlockData::eatClosePad() {
	int offset = -1;
	for (int i = tokens.count() - 1; i >= 0; i--) {
		if (tokens[i] == BlockData::Token_OpenPad) {
			offset = i;
			break;
		}
	}
	if (offset >= 0)
		tokens.remove(offset, tokens.count() - offset);
}

void BlockData::eatCoreDelimiter() {
	if (tokens.count() && tokens.last() == Token_CoreDelimiter) {
		tokens.remove(tokens.count() - 1);
		tokens << Token_Core;
	} else {
		tokens << Token_CoreDelimiter;
	}
}

void PadHighlighter::highlightBlock(const QString &text)
{
	QTextBlock block = currentBlock();
	BlockData *blockData = dynamic_cast<BlockData*>(block.userData());

	QTextBlock previousBlock = block.previous();
	BlockData *previousBlockData = previousBlock.isValid() ? dynamic_cast<BlockData*>(previousBlock.userData()) : 0;

	BlockData *newBlockData = new BlockData;
	if (previousBlockData) {
		*newBlockData = *previousBlockData;
	}

	for (int i = 0; i < text.count(); ++i) {
		if (text[i] == PadAnalyzer::padOpenDelimiter) {
			setFormat(i, 1, _padDelimiterFormat);
			newBlockData->tokens << BlockData::Token_OpenPad;
		} else if (text[i] == PadAnalyzer::padCloseDelimiter) {
			setFormat(i, 1, _padDelimiterFormat);
			newBlockData->eatClosePad();
		} else if (text[i] == PadAnalyzer::coreDelimiter) {
			qDebug("ok");
			setFormat(i, 1, _coreDelimiterFormat);
			newBlockData->eatCoreDelimiter();
		} else if (newBlockData->tokens.count() && newBlockData->tokens.last() == BlockData::Token_OpenPad)
			setFormat(i, 1, _prefixFormat);
		else if (newBlockData->tokens.count() && newBlockData->tokens.last() == BlockData::Token_CoreDelimiter)
			setFormat(i, 1, _coreTextFormat);
		else if (newBlockData->tokens.count() && newBlockData->tokens.last() == BlockData::Token_Core)
			setFormat(i, 1, _postfixFormat);
	}

	bool refreshNext = blockData && blockData->tokens != newBlockData->tokens;
	block.setUserData(newBlockData);
	if (refreshNext) // toggle block state is enough to re-compute the next block \o/
		setCurrentBlockState(currentBlockState() == -1 ? 0 : -1);
}

void PadHighlighter::init()
{
	// init some formats
	_padDelimiterFormat.setForeground(Qt::darkRed);
	_padDelimiterFormat.setFontWeight(QFont::Bold);
	_coreDelimiterFormat.setForeground(Qt::darkRed);
	_coreDelimiterFormat.setFontWeight(QFont::Bold);
	_prefixFormat.setBackground(QColor(220, 220, 255));
	_postfixFormat.setBackground(QColor(200, 255, 200));
	_coreTextFormat.setBackground(QColor(255, 200, 255));
	_coreTextFormat.setFontWeight(QFont::Bold);
}
