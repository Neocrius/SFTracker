#pragma once

#define MAX_UNDO_STEPS	1000

namespace sft
{

class UndoAction
{
public:
	typedef std::shared_ptr<UndoAction> P;

	virtual void perform() = 0;

	virtual void undo() = 0;
}

class UndoHistory
{
public:
	UndoHistory &singleton();

	void addAction(UndoAction::P command);

	bool canUndo() const		{ return _head != _history.begin(); }

	void undo();

	bool canRedo() const		{ return _head != _history.end(); }

	void redo();

	void resetModified()		{ _commited = _head; }

	bool isModified() const		{ return _head != _commited; }

private:
	UndoHistory() {}

	~UndoHistory() {}

	std::list<UndoAction::P>			_history;
	std::list<UndoAction::P>::iterator	_head;
	std::list<UndoAction::P>::iterator	_commited;
}
	
}	//namespace sft
