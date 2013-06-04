#include "Options.h"

const int Options::recentCount = 6;
QSettings* Options::settings = NULL;
QString Options::recentFiles[Options::recentCount];

SoundButton::_DONE_ACTION Options::defaultDoneAction = SoundButton::DONE_STOP;
SoundButton::_RELEASED_ACTION Options::defaultReleasedAction = SoundButton::RELEASED_CONTINUE;
SoundButton::_REPRESSED_ACTION Options::defaultRepressedAction = SoundButton::REPRESSED_STOP;
//*/
