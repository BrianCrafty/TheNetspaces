Introduction

TheNetspaces is a sophisticated and extendable software. It is a single webpage that can be loaded by the browser from the local file system and from the server. The software is built from many functional classes that provide various functionality. The system is under constant development and new functionality appears constantly and we will try to update the code and documentation. Usually the newest version is the most actual, functional and comfortable to use - it contains the newest functionality, bugs and mistakes from the previous versions are usually solved in the newer version and it is the best choice to launch the newest version. Nevertheless, like with any other software, it's definitely not a bad habit to keep also the previous versions if possible.
That is the first attempt to write the concise The Netspaces first of all the user and later also the programmer documentation. People around the world contributed to the software, and science development in general, but there is definitely no point to formulate the manypages thanks - in fact usually they don't want to be explicited  at all. Strong economy, low prices and high prizes will thank them. By that opportunity we only want to mention the Tel Aviv team collaboration with the Warsaw team, especially Benjamin and Edyta from the Warsaw side - Edyta is probably one of the team members that made such huge breakthrough by defining the "Polish Programmer Keyboard" where for the first time for the Polish language we could use the full US keyboard only with pressing Alt (Alt Gr) for the Polish specific dots and dashes, instead of typing the chars on the whole keyboard so different from the US keybaord. That was a huge breakthrough in the Polish Computer Science and all the time we are very contingent on the civilization that types and entertains comfortably, quickly and proficiently in a "programmer mode" - we need such casual civilization so much and therefore e.x. the texteditor with the "programmer mode", not only "simple mode" is a default game approach - such texteditors should be accessible possibly easy for anybody who would wish it - such software is unfortunately often absent on many systems. Anyway, the "programmer approach" is default in the whole game. We mention the collaboration with the Tel Aviv team purely technically, to inform more or less what programming school has been applied between the lines and lies (if it applies in the case at question).
The Netspaces is an interactive game where you can do the number crunching in the attractive 3d worlds, build, save and load. It has many extensions, e.x. Computer.

The main webpage TheNetspaces.html and TheNetspaces_oneliner.html should be equally functional. TheNetspaces.html is a code with newlines and TheNetspaces_oneliner.html is the same code without newlines. That text file has only one line. Often it is extremely functional and additional effort and programming practices must be often put, so that the many line code version can be also formulated in form of a code oneliner.

Other files in the directory:
-dictionaryEditor.html and dictionaryEditor_oneliner.html - to edit the languageDictionary and the keyboardDictionary used by the texteditor Computer program.
-extensions directory - directory for the texteditor extensions - scripts to run on the text and dictionaries e.x. bothDictionaries.txt to use by dictionaryEditor and TheNetspaces texteditor program.
-injections directory - directory for the injections extensions-ShaderThinkpadShaders directory - directory for the ShaderThinkpadShaders

File input output

In many situations you have to load files with the input data and save the output data into the files. When you load a file the webpage opens you a file input dialog so that you can load a file from your filesystem. To save the file you usually press some button to prepare the file content in the memory and when the file is ready, the download link near is prepared or it is downloaded automatically, that has equal effect if you press that download link. Usually downloads are automatically downloaded by your webbrowser in to specified directory, e.x. Downloads directory. Often when you want to "Save" in fact you have to "Save as" it again. When a webbrowser downloads another copy of the file with the same name it calls it in a form:
filename (number).extension
where number is consecutive number of the file with that name in the directory. Since it is often good to save the work possibly often, in fact you end up with many previous versions of the saved files in the directory with high number. It can be comfortable to remove the previous versions regularly, leaving only the actual one and move them from the e.x. Downloads directory to a directory in your directory structure. Naturally sometimes it can be comfortably to leave the changes history. Sometimes, if you have enough space in the filesystem, it can be comfortable to remove all the previous versions at the end of the work. Usually you can precise the name and extensions, but sometimes automatically names and extensions are chosen, and you have to rename them also. The file operations have to be performed with external operating systems tools - usually if you have a webbrowser in the system compatible to run that code, you also have the file operations tool, some file manager etc. From the security reasons, the file operations can't be realized otherwise. On some less advanced systems you may have difficulties to rename the extensions, so you have to precise them correctly when saving - with different extensions content will not be opened by the app you desire.

Computer

This is that in-game mini laptop computer where you can run the programs. Programs can be launched in the text mode and graphical mode. If you will not set up the blnTextEditorOnStart=true;, the global configuration variable like described in the Initial configurations section, computer will start with a commandline, where you can run commandline commands and programs.

Computer commandline

Internally it is realized the Program_Bash class instance. It always needs a base - weather it is Computer class itself or Program_Terminal in the startx graphical XWindows environment. That is the underlying very basic program below. If in the initial configuration variables in the blnTextEditorOnStart set to true the  Program_TextEditor is not specified to be started automatically after the webpage opening, the Program_Bash is ready to use. Instance of the Program_Bash is always below as a base of any program - weather startx and further through Program_Terminal or directly in the programs. You exit to commandline and you can't exit the commandline itself in the text mode (you can exit the Program_Terminal and the whole Program_startx landing back in the most below Program_Bash commandline).
From the commandline, the Program_Bash instance you can start the programs and commandline commands. The commandline commands are following:
We use here convention: whenever you will enter command (or program name) with the arguments, "command args" into the commandline, we will denote is as
"#command args"
to emphasize that it is about the commandline Program_Bash command.
So the following commands are available:
"#clear" - clears the content of the text screen on the screen or on the terminal.
"#fg "+number  - resumes the stopped process with given number. To see all the processes type the command "jobs".
"#jobs" - shows all the processes in the commandline. 
Usually there is a convention. You can close the program with "ctrl+c" or with the "F4" key ("alt+f4" is often used to close the whole browser). You can stop the program with "ctrl+z". That is not a must, but the minimal program template Program_txtFunFunFun that you can copy and extend constitutes such conventions, that lot of user is used to. Like written, once stopped the program you can resume it with the "fg" command.
"#echo "+string - writes the string on the console.
"#date" write the actual timestamp on the console.
"#new "+the program name - starts new program.
"#interpret "+the string - interpret the string as the JavaScript.
Those commands are defined explicitly as the functions with the conventions "#functionName" in the Program_Bash.prototype.functionName. Other functions from the Program_Bash are internals. List of all commandline commands is available in Program_Bash.lstShellCommandNames for the names to enter, the function references are in the Program_Bash.lstShellCommands.
Programs are launched by the base (computer or Program_Terminal) and they are taken from the installed programs list. Running programs are stored in the base.lstRunningPrograms variable and program names are taken from the base.lstProgramNames. 

To write your own program into TheNetspaces.html, the fastest way would be to copy the Program_txtFunFunFun template, extend the function with the functionality like the other programs, and than install programs, e.x. by extending the Computer.prototype.inits function with another installProgram line:

Computer.prototype.inits=function(){
	this.installProgram(["snake",Program_SnakeGame]);
	this.installProgram(["startx",Program_startx]);
	this.installProgram(["txtFunFunFun",Program_txtFunFunFun]);
	this.installProgram(["binarysnake",Program_BinarySnakeGame]);
	this.installProgram(["texteditor",Program_TextEditor]);
};
that installs program class under chosen name in computer. In installProgram function the first argument is the commandline name and the second argument is a name of the program class kept in the convention Program_ProgramName. The class can be defined in the webpage code itself or in the injection file. You start the above programs with the name and arguments specific for given program e.x.
"#texteditor -maxi -O"
Arguments are often interpreted in inits, where they are compared with previous arguments and often functions like reloadStrArgs() or analyzeCommandline() are entered. inits() is a good place for commandline parsing, since it if started after the object is created from constructor in memory.

Computer program template 

Example, minimal ready to copy and extend program template is Program_txtFunFunFun. Programs have few usual functions that handle usual behavior. More about it will be written later.
Usually the configuration variables can be find in constructors.

Computer texteditor

The texteditor uses both, most popular in the Computer Science history standards, that, like the languages, constituted itself up to now and are applied daily by millions of users worldwide - you can use it in a "simple mode" and in a quick and much more comfortable "programmer mode", that you first have to get used to once in a lifetime, but when you are already used to that standard, it is very quick and comfortable.
You can open it in the commandline without any arguments:
#texteditor
Usually it is not what you want, since it opens the texteditor in the "simple mode", without whole advanced functionality that is the biggest advantage of that soft. Worth to mention, that txtFunFunFun with the basic file loading and saving and downloading functionality can me much more comfortable on devices where you have non precise touchscreen instead of mouse pointer. Touchscreens are often optimized for a system textareas and you can also use the webbrowser search, clipboard etc. functionality. "Simple texteditor mode", without the "programmer keyboarding" is designed for use with menu, where mouse is comfortable. If you don't have precise mouse (try with the 3d mouse option on and off) you can use "ctrl+esc" key to open the file menu and arrows to move between menus and in the menu itself. Left and right arrows to change the menu, up and down arrows to highlight another menu option. If you have submenu with the arrow -> you can press enter to enter it - if it is not submenu, i.e. without arrow ->, pressing enter will choose the option. You can close the submenu by pressing "Escape". Closing the last menu will close the menu and you will jump back into the active editing window. Since menus are quite self explaining, we will first describe other texteditor modes.
With the command:
"#texteditor -maxi"
you can open the texteditor in the "programmer mode", i.e. with the previous menus above and also with one line of the commandline below.
"#texteditor -mini"
would open the texteditor in the "programmer mode" without the above menus - only the below one line of the commandline. Option ideal for the tiny screens. Generally the "programmer mode" is very handy for quickly typing on the modern touch screen keyboards, that are usually reduced to fit in the small touchscreens.

What is more, in the "programmer mode" the texteditor has two modes - the command mode and the editing mode. Initially it is in the command mode, where you enter the commands. To enter any text into the active textediting window you have to enter the editing mode. You can do it in various ways.
You can press "i" on the keyboard to go into the editing mode and start entering text just before the cursor in the active editing window. You can press "I" to move the cursor to the beginning of the line, go into the editing mode and start entering text there. You can press "a" to start entering text in the editing mode after the cursor. Analogously "A" with the end of the line. Other commands that start editing mode:
"o" - make a newline after the line with the cursor and start entering text in the editing mode,
"O" - line before the with the cursor.

To exit the editing mode, where you write into the active editing window you press "Esc". If you don't have that very important key on your keyboard, you can use the keyboard drawn in the Computer Screen Mode. There is also quick to use button in the Artificial textarea that you have to open by setting the checkbox. 

There is one more mode, very similar to the editing mode - the replace mode. You can enter it from the command mode with "r" button - then you will replace only the letter under cursor with "normal keys" and immediately exit it after one keypress. You can also enter that mode with the "R" button from the command mode. They you will be replacing consecutive letters with "normal keys" one by one with consecutive keypresses. Pressing enter will insert enter just in the editing mode, i.e. it will break your line into two. Pressing "normal keys" consecutive keys will be replacing the text. Again, you exit that mode by pressing "Esc".

Internally that were the socalled "0modeCommand", "insertMode" and "replaceMode" modes.

To move the cursor around you can use arrows. That will move the cursor also in the "simple mode" and in "programmer mode" it will move the cursor both in "editing", "replace modes" and in the "command mode". In the "command mode" there is much more comfortable way to move the cursor, namely with the keys "h","j","k","l", so that you don't have to get the hands from the standard readiness position on the keyboard, i.e. with both pointing fingers on "f" and "j". That is also a great option for the touchscreen keyboards in modern smartphones, where you usually don't have arrows. "k" will move cursor line up, "j" line down, "h" char to the left and "l" char to the right. You can also move the cursor to the line beginning with "^" and to the line and with "$". You can also move the cursor the whole word backwards - "b" and the whole word forwards - "w". Generally in the "0modeCommand", the command is internally built by the command builder. If you first enter digits, that first digits will be interpreted as a command counter, often like it would repeat that command that many times. So "10j" will move ten lines down, "10b" will move en words backwards. "10^" will have equal result like you would press it once. Whenever you enter the text or move the cursor, first positions are stored on the stack. After stacksize limit the history of the positions is forgotten. To move to previous position you can press "ctrl+O". To move to the next position you can press "ctrl+I". Analogously with the undo redo history, unless you will turn it off, like it is often desired e.x. in the automatic text crunching scripts with the fastcruncher, you can also undo and redo what you type in the editing windows. If you press "u" in the "command mode" it will undo last action. The repeating actions are glued together, e.x. if you type text constantly, pressing "u" will remove that whole text excerpt, not letter by letter. That undo entry is moved to the redo entries queue and after undo you can redo them with "ctrl+r" until you start consecutive editing actions. So e.x. you can test the text with "u", "ctrl+r" and move cursor around, but if you enter the text again, undo item is created and the redo items are removed. "u" and "ctrl+r" also move the cursor.

To remove the text you can use backspace and delete in the "editing mode" before and after the cursor. "Simple mode" has only the "editing mode" and so almost all the commands from the "editing mode" should have equal result in the "simple mode". In the "command mode" you can delete the character under the cursor with "x".

All deleting commands in the "command mode" can be repeated with the command counter and deleted content is copied into the clipboard 0. In that texteditor version "x" is exception but will also have that functionality in new versions.

You can also delete text with the following commands:
"dd" - delete the whole line under the cursor. With command counter it will remove also the lines below.
"dk" - delete the line above the cursor. With command counter the lines above.
"dj" - delete the line below the cursor. With command counter the lines below.
"dh" - delete the char before the cursor. With command counter the chars before.
"dl" - delete the char after the cursor. With command counter the chars after.
"db" - delete the word before the cursor. With command counter the words before. The content after is not deleted, so with cursor on the word will leave the part of the word under the cursor and after.
"dw" - delete the word under the cursor. With command counter also the words after. The content before the cursor is not deleted, so with cursor on the word will leave the part of the word before the cursor.
"d^" - delete the chars from the place before the cursor to the line beginning.
"d$" - delete the chars from the place under the cursor to the line end.
You can imagine how worksaving it is, e.x. when working with the code oneliners.
You can also delete selection with "ds". The selection is highlighted with "shift+arrows" or the mouse drag - double click quickly with the left button and move the mouse when holding the left mouse button from the second click - single mouse click in the text will move the cursor to position under the mouse click. Since the most behavior is often buggy and on some devices is is hopeless now, very useful command in that context is "ctrl+o" to return to the previous cursor position, or equally with "u" that will also set the previous mouse position apart from undo previous operation. That all with the selection great works also in the "editing mode" and thus "simple mode" - the selection is highlighted with "shift+arrows" or the the mouse drag. In case of problems try the variants with the 3d mouse on and off. The 3d mouse is generally meant for working with the laptop geometry in the 3d The Netspaces words, when normal mouse is meant for working in the screen modes.
You can also use the marks systems in various operations, also for deleting. To set a mark you go to the desired position and press "m" plus the letter a-z denoting the mark. E.x. "ma". Then you can delete from e.x. from the actual cursor position to the mark with "d'a" where the single quote denotes the actual cursor position and a stands for the mark you set. 

Important to mention, that if the actual cursor position is in different line than mark a, it will delete all the lines between including the whole first and the last line, no matter weather the actual cursor position and the mark are at the lines boundaries or anywhere in the middle.

Analogously to move cursor into the marked position, the goto command can be used, e.x. "g'a" - go from the actual cursor position to position marked by a. With the goto commands also worth to mention "G" (i.e. "shift+G") that means goto the end of the text, to the last line. To go to any other line than last with the "G" command you need to enter the command counter first. So first you enter digits that the command builder collects like the calculator e.x. first thousands, then hundreds, then tens and then 0-9, that you naturally press without "shift", and then "shift+G", i.e. "G". That will move you to the line under the cursor. By definition the first line is line number 1 (not 0) and the last line is shown in the bottom right corner. Information written there is in form rowChar/line length, actual line/line count. The plus after line count indicates weather the text has been modified and not saved yet. When you load the text and e.x. only read it, there is an empty space. If you make any modification, also when you undo it, that plus remains until you save it and download it or load another text.
Other goto commands are for example goto definition
"gd"
that gets the word of the cursor and searches for the first occurrence in the text file. It searches from the first line. If it finds it it highlights all the occurrences of that string and goes to the first occurrence in the text file. If the word under cursor is the first occurrence of the string in the text file, it goes to the first letter in that word.
Other goto occurs in "%" if you are on the bracket. Than if the pairing bracket will be found in the text, for the bracket under the cursor, than the cursor will move to that pairing bracket. That is the same algorithm that highlights the pairing brackets. Also the find commands move the cursors to the occurrence of the found string. To change the search string and find for the first time you type "/" to enter the "2modeCommand" like described below, write the string and press "enter". It will move you forwards to the closest occurrence of the string next word, after the word under the cursor. If it will not find such expression to the end of the text, if will start searching from beginning, from the line number 1, so you may land also above the cursor. When you have defined the search string you can simply press "n" in the 0modeCommand to search for the next occurrence of that search string forwards and "N", i.e. "shift+n" to search backwards. Like with searching for the first time, the searching is cyclic, i.e. with searching forwards after the text end the text will be searched from the first line and with searching backwards, after the first line, the text will be searched from the last line. The found text is highlighted in the text if the highlight option is turned on - that is default setting. If the search string is not found, the information
"String not found"
will be written on the comamndline until you press anything. Search string highlighting is defined per window with its blnHighlightSearch variable and you can set it by typing in the "command mode"
":set blnHighlightSearch=false"
like you can set any window variable from commandline. There are also handy aliases for that command, so that if you are used to that convention, you will be happy to find that option also covered that way here:
":set hlsearch"
instead of
":set blnHighlightSearch=true"
":unset hlsearch"
instead of
":set blnHighlightSearch=false"
Also
":set nohlsearch"
and 
":set blnHlsearch=true"
":set blnHlsearch=false"
are artificially defined, but ":set blnHighlightSearch=" is compatible with general setting any internal variable - there is no such variable like blnHlsearch, it is only artificial alias to maintain the compatibility with other software.
It is often, that with the most popular commands different, most popular variants are defined for users used to different conventions. Some have to be defined differently than usually, since such conventions are already defined in the most popular webbrowsers and operating systems, but with luck, it is not so often. These are the "1modeCommand mode" commands, described below.

Very similar set of commands to delete commands, are the copying commands. What can be different from the standards you are used to is using ten clipboards from default 0 to 9. That is very handy, but after every copy and after every paste command you have to enter digit, specifying the clipboard number - without that, the command builder does not recognize the copy paste command and if you start entering the next one, command builder will be still building the command and when you have realized that your commands stopped working, you have to "Escape" it to enter the correct commands again.

To copy, otherwords to yank the whole line under the cursor to the clipboard number 0 you have to enter in the "0modeCommand" "command mode"
"yy0"
or
"cc0"
If you want to copy the whole under the cursor and the line below to the clipboard number 0 enter
"2yy0"
or
"2cc0"
With clipboard different than 0, enter digit 1-9 with the chosen clipboard number.
So the copy command kernels:
"yy" or "cc" - copy the whole line under the cursor. With command counter it will copy also the lines below.
"yk" or "ck" - copy the line above the cursor. With command counter the lines above.
"yj" or "cj" - copy the line below the cursor. With command counter the lines below.
"yh" or "ch" - copy the char before the cursor. With command counter the chars before.
"yl" or "cl" - copy the char after the cursor. With command counter the chars after.
"yb" or "cb" - copy the word before the cursor. With command counter the words before. The content after is not copied, so with cursor on the word will leave the part of the word under the cursor and after.
"yw" or "cw" - copy the word under the cursor. With command counter also the words after. The content before the cursor is not copied, so with cursor on the word will leave the part of the word before the cursor.
"y^" or "c^" - copy the chars from the place before the cursor to the line beginning.
"y$" or "c$" - copy the chars from the place under the cursor to the line end.
You can imagine how worksaving it is, e.x. when working with the code oneliners.
You can also copy selection with ds. The selection is highlighted like written in the delete section.
You can also use the marks systems in various operations, also for copying. Marks are also described in the delete section. So such copy kernel to copy from actual cursor position to mark a would be
"y'a"
and again, if the actual cursor position is in different line than mark a, it will copy all the lines between including the whole first and the last line, no matter weather the actual cursor position and the mark are at the lines boundaries or anywhere in the middle. So, for copying from the actual cursor position to that marked by the mark a, or most often from the actual line to that line marked by the mark a you would type
"y'a0"
where you have chosen clipboard 0. There is no point to write the command counter here.

To paste from clipboard 0 you would press "p0" to paste single line content after the cursor, multiline content below the line under the cursor or you would press "P0", "shift+P" and 0 to paste the single line content after the cursor, multiline content above the line under the cursor. 

Note, that if you are copying to the default clipboard 0, the content in question also replaces the content of the txtFunFunFun. The clipboard 0 is in fact txtFunFunFun - if you didn't copy anything to the clipboard 0 before and if you paste from clipboard 0, txtFunFunFun content will be pasted. If you use other clipboards 1-9, txtFunFunFun remains unchanged, but since clipboard 0 is default and using deletion operations in the "command mode", you are replacing txtFunFunFun. If there is any significant content in the txtFunFunFun you would like to keep for longer period of time, consider copying if to one of the clipboards 1-9. Applying txtFunFunFun heavily txtFunFunFun can be therefore more tempting, because default content of the active editing window is initially filled with txtFunFunFun content and when you reload the page many times the memory content is reloaded with default content, when webbrowsers often maintain the content of txtFunFunFun from the previous sessions. What is more, txtFunFunFun is a convenient external clipboard to quickly share the content with external applications, beyond The Netspaces with the system clipboard. Naturally the other option is to save the content and download to the file system and open with another application, but clipboard approach can be faster and more comfortable due to habits.

Also note, that external text can sometimes lose the tab indent. To indent the many lines at once, instead of moving to the beginning of every line and pressing "tab", there are two, very useful commands. You can indent number of lines after the cursor if you know exactly how many lines after cursor you want to indent. E.x. to press tabulator after 10 lines you would enter
"10>>"
To indent them back, i.e. press "Backspace" or "Delete" near the line beginning to remove single indent you would enter
"10<<"
If you don't know how many lines you want to indent, you can set mark at one place, e.x. "ma" with the mark a, go to the second endpoint of the range, where you want to indent the lines and type
">'a"
that means indent the lines from the line under cursor to the line marked with the mark a. Analogous the inverse:
"<'a"
that means indent back the lines from the line under cursor to the line marked with the mark a.

In the actual version there is a bug that with ">>" and "<<" also the previous line before the line under the cursor is indented and that indenting is not added to the undo redo queue, therefore undo redo influences the previous operation instead of intent. It will be corrected in the next version.

There are also commands with the internal distinguishement "1modeCommand" and "2modeCommand" - from the user perspective that is all the "programmers mode" "command mode", although they are quite distinguishable. Commands from 0commandMode are not printed on the screen, i.e. the command string from the command builder is not printed on the screen, only the effect of it when the command will be recognized. If the user is not sure what command is in the command builder, "ESC" should be pressed and the command should be written again. The same holds, when the entered command does not bring desired result - than something must have gone wrong and the content of the command builder should be cleared with "Esc" possibly quick before it spoils something. Usually "0modeCommand commands" are short, so there is no problem with that. "1modeCommand" and "2modeCommand" commands are visible in the commandline. To enter the "1modeCommand mode" you have to press colon ":" and after that the colon ":" appears in the commandline. Everything you type appears commandline, the command builder builds the command char by char, you can use backspace, delete, move the cursor with arrow left and right. Up and down cursor search the commands in the command history of the session. When you "Esc" you cancel entering the command and clean the commandline, exit the "1modeCommand" - you are again in the "0modeCommand". When you press "enter" instead of "Escape" you enter that command to the interpreter. That command - correct or not - is added to the command history. Next time when you move up wih the arrows in the 1commandMode you will replace the actual commandline content with that command. The lowest command is the command not stored in the history queue, the actual command you started edit. Nevertheless you can decide to edit command from the history - all in all when you enter it, it will be added to the end of the queue. "1modeCommand" is the most extendable "command mode" - from external commandline commands to user defined text crunching function names from extensions. These are usually longer commands, but not always.

Commands always have effect on the active editing window (and shared buffer windows), so if you have many windows and want to enter the command on another window, you have to move to that window. Usually you do it with "ctrl+arrows" (window left, up, above, below) or with the mouse click on that window. That way you change the active editing window. With shared buffer windows you understand windows with the same file opened in both windows - then they are marked as the share buffer windows and practically any content modifying keypress, any modification of the text buffer is performed on that buffer and the shared buffers. Internally in is done often on the low level - commands are often written for the active editing window and on the low level buffer modification function, history functions etc. repeat the operation from the buffer on all shared buffers.

The most important "0modeCommands":
":o" - open - open the text file in the active editing window. It opens the file dialog, where the file from the file system can be chosen.
":n" - new - fill the active window with the empty content.
If the last editing window is closed, the whole program is closed. In the future versions there will be warning, if the file has been modified and not changed (+ in the bottom right corner), weather to save the content from the actual editing window or use the force commands, i.e. with exclamation marks like  ":o!", ":n!" etc.
":w" - write. Writes the content of the textbuffer to the download file and automatically downloads it. That is "Save as" command, where you create new file in the file system. From the security reasons you can't have "Save" command. See the "File input section" for details. In another text editors it is usual to use ":w" as the "Save" command and ":w" filename as the "Save as" command. Here with the download option there is only "Save as" command. If you will write only ":w" then the file will be saved under default name, i.e. TextEditor.txt. Is such file already exists in the folder set for downloads in the webbrowser, usually set as Downloads, the name, depending on the webbrowser, will look like "TextEditor (number).txt" (often to access it you have to use the escaped sophisticated characters, like "TextEditor\ \(number\).txt", e.x. in the system commandline). You can also write
":w filename.extension"
and it will download the file saved as filename.extension eventually, depending on the webbrowser filename (number).extension . Nevertheless, it is a "Save as" functionality and you have to replace previous version of the file with the new one with the external system tools.
 
":q" - quit - tries to quit the program. If you have any unsaved work it will display the message
"File not saved. Force closing with !" and you have to save all the unsaved files from all the editing windows or force closing with ":q!" that will close with losing the work. 

The above commands have the following aliases to hold also with other popular conventions:
-":n", ":new"
-":o", ":open",":read",":load",":upload" - in the future versions "read" will be used for inserting external content.
-":w", ":write", ":save", ":saveas", ":download"
-":q", ":quit", ":exit",":close" and analogously ":q!", ":quit!", ":exit!",":close!"  - in the future version we will consider distinguishement between closing the whole program and only the editing window. 
Now for closing the editing window the ":win--" or the menu entry is used. Analogously ":win++" for opening new window.
Other commands:
-":oshd" - open actual shader text for the Shader Thinkpad. More details about the Shader Thinkpad below.
-":wshd" - compile the actual shader  for the Shader Thinkpad.
-":owshd" - open and compile the shader at once for the Shader Thinkpad.
-":initFastCruncher" loads the initFastCruncher function with the fastCruncher engine great for the external scripts.
-":extensions++" opens the file dialog to load the file with the texteditor program extensions (naturally different that The Netspaces extensions like computer, piano etc. from the injection files) - that can be also done from the extensions menu. If the extension code is correct (more details in the texteditor extensions or TextEditor extensions section and example extensions in the extensions directory. Convention - TextEditor like names stand for the class, texteditor like names stand for the object, i.e. particular realization of the abstract class in the memory.), that the extension function name is added to the menu. Nevertheless you may wish to use many help functions to modularise the computations and it is well to remember the main entering function name then to run it from the commandline. There is no ":extensions--" - restarting the webpage starts the session without the extensions in the memory.
":=>" and the extension function function name will launch the extension from the commandline. It is good idea to use the initFastCruncher function there instead of doing that from commandline.
There is no way to run the freeFastCruncher command that rolls back the settings from the initFastCruncher functions with the freeFastCruncher function. Naturally you can always quickly launch that function from the extension file or the console. Anyway, such "1modeCommand" will be added in the future versions.

":!" - run external command. Launches the external command in the background waiting for it end. It shows the commandline content and after keypress it goes back to the editing text without inserting the result.
":r" - insert external content from the file. It opens a file dialog with the file to insert under the line under the cursor.
":r!" -  Used for the commandline outputs of the external programs and commandline commands. It inserts that output under the line under the cursor. E.x. ":r!date" will insert the actual date from the commandline command "date".

":set wrap", ":set blnWrap=true" will turn on the text wrapping.
":set nowrap",":set blnWrap=false" will turn off the text wrapping. If there are some errors with the wrapping, the nowrapping mode is the simplest mode.

Because of reduced screen sizes, tabulator is by default set with the alias char. The alias char is by default set to that arrow sign:
"that.chrTabAlias=String.fromCharCode(10148);"
You can also set it from the commandline, like every internal variable, for example for the normal tab empty space:
":set chrTabAlias=String.fromCharCode(9)"
The graphical font is fixed size, so the tab can be quite small. In future versions we could e.x. consider expanding tabs into spaces - such option is often accessible in other text editors since many documents in the text editing world are formatted that way. Naturally already you can press spaces instead of tab.
Sometimes highlighting the tabs is desired to increase readability.
":set hltabs",":set blnHltabs=true","set blnHighlightTabs=true"
will set highlighting tabulators from the indent. Turning them off is possible with:
":set nohltabs",":unset hltabs",":set blnHltabs=false",":set blnHighlightTabs=false"

Analogously with the already mentioned highlight search option, that will highlight all the occurrences of the found string:
":set hlsearch",":set blnHighlightSearch=true",":set blnHlsearch=true"
and
":set nohlsearch","unset hlsearch",":set blnHighlightSearch=false",":set blnHlsearch=false"

There is also a great option, that sometimes is implemented on the operating system level, namely the programmatic shift. It's a great option if you e.x. like to type with one hand. That is a great to type in bed, when doing other things etc. People often train left hand for that purpose because right uses mouse simultaneously. Programmatic shift allows to use CapsLock not only for letters, but also for symbols. In normal mode you have to press and hold shift in order to write e.x. the symbols that are on top of the digits. It doesn't matter weather your caps lock is turned on or off at that time. With the programmatic shift turned on you can write that symbols without having the shift pressed at that time. It is sufficient, that the capslock is turned on, just like in case of letters.
":set Shift_Lock",":set blnProgrammaticShift_Lock=true;",":set blnShift_Lock=true" turns it on
":set noShift_Lock",":unset Shift_Lock",":set blnProgrammaticShift_Lock=false",":set blnShift_Lock=false" turns if off
By default it is turned off, like with every variable you can set it in the constructor in the webpage text to turn it on.

There is also option to set key aliases for non typical or keyboards with broken often used key. Especially on the on screen mobile keyboards it can be very handy function, e.x. to use "~" as "Escape", that is often missing - naturally there is that quickly accessible "Esc" button in the artificial textarea section. For such tilde to use as escape key you would type into the commandline:
':set key "~"="Escape,Escape"'
(with those double quotes inside).
You can also unset all the previously set key aliases with:
":set nokeys",":unset keys"
Since setting the keyaliases can be not comfortable every time when you start a new session, there is convinient place in the whole The Netspaces webpage html file, especially meant for that purpose, namely that.initialCommands list with strings with all the commandline operation run at the start of the application. E.x. to set up such programmatic alias for tilde you can set:
'that.initialCommands=["set key "+String.fromCharCode(34)+"~"+String.fromCharCode(34)+"="+String.fromCharCode(34)+"Escape,Escape"+String.fromCharCode(34)+""];'
in the constructor. Constructor is usually a good place for initializations - lot of default values for the variables is set there, there are also many examples of alternative options in comments. Some initializations are better to performed when the object is already instantiated - in constructor the first instanciating takes place. If you want to perform some inits after the object has been instantiated - usually that is a case with the programs - then there often second functions where lot of initializations is performed, namely inits() function. That function is usually called when the object has been already created. That is often case when e.x. instead of closing program with "Ctrl+C" or "F4" - "Alt+F4" closes the whole browser - you press "Ctrl+Z" to stop the program. Then the program remains in the list of active jobs, that you can view from console with the "jobs" command and run again with the "fg" command. At that point the inits() function is often entered. It is often, that there the strArgs are analyzed again. Some behavior is different from the standard behavior, but it can be great e.x. for the ingame apps - here also the mobile systems world was an example.

You can work on many windows. You can add windows from the menu or with the commandline command:
":win++"
To close window you can use:
":win--"
Also if you open the texteditor with the "-O" or "-2vwin "in the Program_Bash you can open it with two vertical already aligned widows and with the "-o" or "-2hwin" option in the Program_Bash you can open it with two horizontal already aligned windows. With the functional call:
":resize(intRootX,intRootY,intWidth,intHeight)"
or with the commandline argc,argv style call:
":resize intRootX intRootY intWidth intHeight"
you can resize window to intWidth,intHeight and move top left corner to intRootX,intRootY. You can also resize the window with the mouse by clicking on the window border. Dragging mouse with one click and move on the single border"- | +" will resize it, clicking on the title bar "=" will move the window. That is exactly the same algorithm like in XWindows in the Program_startx only for chars. Often the biggest problem is that when you drag with the mouse the mouse goes beyond the canvas and the events stop flowing. Then you press left alt to hold the mouse, move the system mouse pointer in different place and press left alt again to enable it again. Depending on the screen dimensions etc. pointers speeds, pointer distances can be different and so it will be essential to use the alt approach, just like in other places in The Netspaces. If it is problematic, the 3d mouse option can be helpful - in the computer screens mode the mouse without 3d mode should exactly stick under the system mouse cursor, so that sometimes it is hard to see that "I" mouse cursor under the system arrow. It is much more comfortable to use mouse in the "Screen canvas mode" than in the "Text screen mode". "Screen canvas mode" is also faster. On the other hand "Text screen mode" can be much more comfortable, will use the font you are used to and in the initial configurations is is default mode. In the "Text screen mode" you can only use "Text mode" applications. XWindows applications can be used in the "Screen cavas mode" and in the "Normal The Netspaces mode", also with automatic camera centering on the laptop screen. 

Worth to mention that in the menu you have also the "Maximize window" option that has no direct counterpart in the commandline args. If you know dimensions of the screen - you can read them from initial configuration on the top of the html webpage if unchanged during the session or with the "Resize text mode screen" in the Computer section in the Extension manager and clicking cancel after reading those dimensions or pressing ok without changing the numbers, than by subtracting 1 for menu and 1 for commandline, you can figure out dimensions to apply with the resize function like above.



The "2modeCommand mode" is very similar to "1modeCommand", but it is much simpler. At the moment is has only one functionality, to search. You enter it with slash "/" from "0modeCommand" and after you press "enter" the text after slash "/" is searched forward in the text. There is similar command editing and command history.

More functionality will be described in the future versions of the documentation. If you can't find some information in the documentation we refer you to the source code, especially with searching for the the onKeyDown and interpret functions. In the text editor worth to search for the onKeyDown_ function and onKeyDownSimple. What you will be probably especially searching trough now will be the Program_TextEditor class and TextWindow class, conceptually and algorithmically similar to the XWindow class from the Program_startx.

TextEditor configuration variables

TextEditor extensions
The Program_TextEditor extensions (naturally different that The Netspaces extensions like computer, piano etc. from the injection files), that can be added from the external extension text files with defined extensions function, with the command:
":extensions++" that opens the file dialog to load the file with the texteditor program extensions. There are also default, defined at start texteditor extensions defined in the Program_TextEditor.prototype.setInternalCodeExtensions. These are that three texteditor extensions in the extensions directory: toCodeonelinerExtension, calendarExtension and whitespacesExtension. They are divided into separate files, but often it is not a bad idea to copy them into one file and load them all with the ":extensions++" at once if they have separate commands. Since they are by default loaded in the setInternalCodeExtensions, they are in fact instructive example, how to write own extensions, since there is no necessity to load that file. Those extension use various techniques like fastCruncher extension interface and also call directly texteditor functions. Extension functions have to be in form "function functionName(that,strArgs){" then the function content and "}". When loading the parity of brackets is calculated and other functions will not be loaded. So if you have any other functions that you don't need as extension, but would like to write as a shared functionality, used by other functions, code them as the following signature or repeat definitions everywhere where required, e.x. as internal variables like "var functionName=function(){}" etc. From the level of the extension you have access to the calling Program_TextEditor through that pointer. You have also access to the arguments string that you have written when calling the extensions from the command line ":=>functionName strArgs". Without arguments the strArgs will be the zero length string. From the extension functions you have also access to global "netspace" object, where the whole TheNetspaces object has been linked. Also "netspace.computer" is accessible etc.

So now, there are three default extensions also in the extensions directory, first is toCodeonelinerExtension.txt
":=>toCodeoneliner"
extension and the second is calendarExtension.txt with the calendar with two commands:
":=>insertDay intMinH intMaxH"
":=>inserMonth intMonth intYear"
Without arguments they will insert - ":=>insertDay" the whole 24 hours and ":=>insertMonth" actual month. The calendar operates directly on the editing text with the Program_TextEditor interfaces. The code oneliner extension uses FastCruncher Program_TextEditor interfaces to crunch the text, since it hanges significantly the whole text and substitutes new content, when calendar only inserts additional content. ":=>toCodeoneliner" can be therefore a little bit deceptive as an example, because initFastCruncher() function defines "netspace.wndEditing=this;", that otherway would not be accessible. You could also directly set it from the extension function like "netspace.wndEditing=netspace.computer.theActualProgram.wndEditing;" if you run it from the commandline, without Program_startx, without "X Windows". Anyway since "that" is in fact equal to "netspace.computer.theActualProgram" in that case, you can also use simply "that.wndEditing" like in the calendarExtension.txt extension.

The calendar extension together with the external content from the commandline inserting, like with date
":r!date"
provide great base for various calendars, schedules, plans etc. Together with the terminal and think pad texteditor it is a great example of sticking to possible modest, universal and extensible standards.

In the externalPrograms there are also source of two C programs with the above calendar functions, that you can compile and e.x. insert into "/usr/bin" and use from the programmer texteditors that have the "insert external content" functionality, e.x.
":r!insertDay 0 23"
":r!insertMonth intMonth intYear"
where the intMonth, intYear are month and year in question. We use convention in the programs and scripts, that months are numbered from 1, not from 0, like later internally in internal conventions for days and months. 

Those programs are itself great examples of the C structural object oriented programming approach. They are also example how to port that texteditor textcrunching extensions into other devices and systems.

initFastCruncher(false); computes on the reference on the orginal text. initFastCruncher(true) copies the text. It turns off the undo redo history by setting 
"netspace.wndEditing.intUndoRedoLimit=-1;"
that you can also set from the commandline like all set internal variables of the editing windows:
":set intUndoRedoLimit=-1"
By default the intUndoRedoLimit is set quite high, i.e. to 1000 entries:
"that.intUndoRedoLimit=1000;"
so if it too much, you can reduce it - it is 1000 optimized entries. Usually it is not a problem nowadays at all. If there are other problems with accumulating content, use copyString function, that will care more than usual for the garbage collection instead of setting just the substring. That problem can be significant with huge fast FastCruncher tasks on huge texts. Good to print the progress e.x. on the console with "console.log" functions, to make sure that the program is responsive. Overestimating computational task e.x. ten times is nothing unusual. On the other hand massive output to the console can be the slowest task from all. After finished computations freeFastCruncher will get back the previous settings like the previous intUndoRedoLimit etc.

The third extension is the whitespacesExtension with two extension functions
:=>noWhitespaces 
and 
:=>whitespacesMM
They both do exactly the same, they remove the C-style programs redundant whitespaces that can be all removed automatically without spoiling the program, first extension function on the string strToModify, to provide easy and portable algorithm, second uses the internal fastCruncher and uses the the internal texteditor stuff. It often turns out, that the first version is quicker. Anyway, possible strArgs in arbitrary sequence:
:=>noWhitespaces emptylines-- withQuotes noComments
:=>whitespacesMM emptylines-- withQuotes noComments
where with emptylines-- you also delete empty or lines only with whitespaces. By default, without emptylines-- option, algorithm leaves that "newlines" for readability. With the withQuotes argument you remove whitespaces also in the quotes. It spoils many programs, but with many programs e.x. only aesthetic appearance will be modified. With the noComments option you leave comments from the whitespace removal, since people usually use the human language there. By default, without the option, comments are also affected, so it would be "emptylines--option" instead of "emptylines-- option". In lot of codes people often write short not complicated phrases and comment out the whole code excerpts, therefore the default settings.
Extension probably will be compatible with all the C, C++, Java, JavaScript, C# etc. codes, although never say never, especially with extensions like documentation tools etc. As usual, like with all the soft, better to make backup in order not to spoil the code in huge automated tasks, eventually finetune the extension functions. Tested mainly on JavaScript yet. Also with the :=>toCodeoneliner extension don't forget, that the code must be specially written to transfer it to oneliner - especially avoid "else enter instruction;" (in case of else without {} brackets) since it will transfer it to elseinstruction and put semicolon after every equation e.x. var testFunction=function(){............}; With the :=> toCodeoneliner backup is also required. Often everything works at once or don't work at all, but with some codes in-depth testing of all the functionality is required. Well, code oneliners are slightly more sophisticated codes than the manyline codes newlines. The Netspaces are written with the priority to be oneliner compatible from the first stage and it's tested in both versions every time. The extensions make the whole code changes and have been tested mainly on TheNetspaces.html.

Code without whites is definitely a matter of personal taste - we personally like that code without whites very much. So dense, so aesthetic, so beautiful, so hackish, so geekish, so "orange". Just like we personally prefer code without code coloring - such a black terminal with the white/gray code on top of it, eventually with orange highlighting, just like in the whole The Netspaces, is what we like so much - already the first look on such screen makes the heartbeats a bit quicker - not only a programming passion - a codebreaking passion :-P A codebreaking seduction :-P

Other code conventions applied in the whole The Netspaces code are not applied in the whitespacesExtension.txt, e.x. instead of
if(intTest==0){
we like
if(0==intTest){
that prevents programmer from accidentally writing
if(intTest=0){
In the second case the compiler will detect the error. 
So the whitespacesExtension.txt isn't "The Netspaces default coding style converter".

It is not bad idea to place all the external extensions in one file and load them at once. Anyway, with default extensions directly in the code, that way, by writing only one command, using the extensions like calendar extensions starts to be very easy and handy and the texteditor can quickly start to be the calendar app.

In the extensions there is a convention that the quick extension documentation is introduced in the first comment after the function definition. In such comment it is quickly explained what does the extension do and what optional strArgs can be applied. They are following:

toCodeonelinerExtension.txt:

function toCodeoneliner_(that,strArgs){
	/**
	* That function is subfunction only - use toCodeoneliner instead.
	* Transform C-style code to its equivalent oneliner
	* one line of code version. In that version it only removes quickly
	* the newlines.
	* Commandline call:
	*	:=>toCodeoneliner_
	* TODO: else enter instruction; and all the = ; checks
	* to ensure that the JavaScript oneliner is always correct
	*/

function toCodeoneliner(that,strArgs){
	/**
	* Transform C-style code to its equivalent oneliner
	* one line of code version. In that version it only removes quickly
	* the newlines.
	* Commandline call:
	*	:=>toCodeoneliner
	* TODO: else enter instruction; and all the = ; checks
	* to ensure that the JavaScript oneliner is always correct
	*/

calendarExtension.txt:

function insertMonth(that,strArgs){
	/**
	* Insert calendar month in the month weekdays view. Part of the calendar extension.
	* Default commandline call:
	*	:=>insertMonth
	* inserts actual month.
	* With optional arguments:
	*	:=>insertMonth intMonth intYear
	* it inserts the intMonth/intYear month. Arguments provided in most popular
	* calendar convention, not in diverse computer science conventions,
	* so e.x. months are numbered from 1 instead of 0 etc.
	* You can also quickly set the formatting of the month by setting the internal strInchySpaces variable, by default set to:
	* var strInchySpaces="   ";
	*/

function insertDay(that,strArgs){
	/**
	* Insert hours of the day. Part of the calendar extension.
	* Default commandline call:
	*	:=>insertDay
	* inserts hours from 00:00 to 23:00.
	* With optional arguments:
	*	:=>insertDay intMinH intMaxH
	* it inserts hours from intMinH to intMaxH, also if intMinH>intMaxH - then the next day hours are inserted.
	*/

whitespacesExtension.txt:

function noWhitespaces(that,strArgs){
	/**
	* Remove C-style programs redundant whitespaces.
	* Default commandline call:
	*	:=>whitespacesMM
	* With optional arguments in arbitrary sequence:
	*	:=>whitespacesMM emptylines-- withQuotes noComments
	* emptylines--: remove also the empty lines or lines with the whitespaces only
	* withQuotes: remove whitespaces also from content inside the 'quotes' and "double quotes"
	* noComments: don't remove whitespaces from the comments 
	*
	* Portable version of the algorithm on the strToModify string.
	* Not fully tested yet, naturally previous backup of the code as always highly suggested.
	*/

function whitespacesMM(that,strArgs){
	/**
	* Remove C-style programs redundant whitespaces.
	* By default
	*	:=>whitespacesMM
	* With optional arguments in arbitrary sequence:
	*	:=>whitespacesMM emptylines-- withQuotes noComments
	* emptylines--: remove also the empty lines or lines with the whitespaces only
	* withQuotes: remove whitespaces also from content inside the 'quotes' and "double quotes"
	* noComments: don't remove whitespaces from the comments 
	*
	* Version of the algorithm on the texteditor fastCruncher.
	* Not fully tested yet, naturally previous backup of the code as always highly suggested.
	*/

DictionaryEditor

In the Program_TextEditor you can also type with the InchesKeyboard to enter languages like Chinese from the loaded "keyboardDictionary". You can turn on/off the keyboard with the check near the backspace in the txtArtificialArea section and with pressing "crtl+space". Not copying and pasting that symbol, to maintain the simple encoding of that documentation for all systems, although for such characters the line
<meta charset="utf-8">
has been set. If it causes problems you could try to remove it, or replace with different encoding, eventually also changing the problematic character, although that should be not big problem, since in the worst case the sophisticated characters will be display incorrectly and the simple character will probably remain unchanged. 

DictionaryEditor is provided as separate webpage. When in The Netspaces the only dictionary used by the Program_TextEditor is the "keyboardDictionary", DictionaryEditor from the DictionaryEditor.html works on two dictionaries: "languageDictionary" and "keyboardDictionary". With the "languageDictionary" you can define handy pocket dictionary e.x. with three languages and insert meanings in the form "meaning0/meaning1" etc. There is a hattrick to avoid commas "," in order not to spoil the whole structure that is the comma separated dictionary. In the "languageDictionary" symbols are the "primary key" that the whole dictionary is indexed upon and thus it has to be unique. In languages like Chinese sometimes there is a case, that one symbol can have two pinyins and two meanings. If you want to add another languages e.x. Japanese that could also use some already defined symbols with e.x. with completely different pinyins, then in order to maintain the very functional simplicity, on can consider adding additional prefixes, suffices, e.x. from the 0-127 character range or predefined phrase, that you can filter out with evey conversion to "keyboardDictionary". There are the following options:
"Phrase to filter out in the keyboard dictionary new entries from repeating ones - info" button, so that you can filter out pharses from suffixes, prefixes in the "languageDictionary" - any number in the phrase e.x. 0 will stand for any number slot [0-9],
"Filter out all 0-127 chars from symbols longer than 1 in the keyboard dictionary new entries" checkbox, that has priority over the phrases, it is applied first,
"Recompute the whole keyboard dictionary when word is deleted" checkbox, so that you can recompute the whole keyboard dictionary when the "languageDictionary" entry is deleted and recompute only new words when the words are added one by one like previously. When recomputing the whole dictionary Ignore unknown pinyins option will really ignore them, they will not be contained in the keyboard dictionary.

Generally bothDictionaries.txt is all what you need to work with - it can store all your linguistic knowledge. You can also hold that copy in the folder - TheNetspaces will automatically recognize that there are two dictionaries. The inches keyboard recognizes weather you have one {} or two {} dictionaries and if there is one it assumes that it's keyboard dictionary, otherwise it assumes that it's bothDictionaries

Pinyins are written in DictionaryEditor in form e.x. DuoChangKuai instead of duochangkuai in the languageDictionary to facilitate the algorithms. In the keyboard dictioanry they have the usual lowercase form like duochangkuai. When you will type the pinyin it shows possible characters in the hints window. If you will type more pinyins than one and have such word in dictionary, e.x. ChengWei it will appear in the hints window after writing chengwei. If you don't have such word in the dictionary and would like to write such two characters, you have to press space after each to write them one by one. If there are very popular although yet slangish, i.e. not systematic, expressions like in Chinese YiDianr instead of YiDianEr, a little bit, YiHuir instead of YiHuiEr, a little while etc. that pinyin will not be recognized, although if it is already in the precomputed keyboard dictionary, it will be copied and you can use the "Ignore unknown pinyins" if you have already loaded bothDictionaries and you are only adding new words with all pinyins correct. First you have to "Save" dictionary - dictionary/keyboard dictionary/both dictionaries - to have anything in the download link to download.

Thanks to the fact that symbol is "primary key", when you will type the symbol it will be automaticaly searched and if found, the other fields will be filled and you can modify them. If no, there will be nothing to fill. If you will press update, in the first case you will update it's content. In the second case you will add new entry. You can remove it with Delete button. After succesfully updating, deleting entry, the buttons will change for a while it's caption to "updated", "deleted". In both cases recomputing the "keyboardDictionary" is essential. When saving bothDictionaries, recomputing the "keyboardDictionary" will be performed. You have to load some initial dictionary, e.x. written by hand in text editor, before you can work with the DictionaryEditor. It can be "bothDictionaries_initial.txt" from the extensions directory.

Other issues in the DictionaryEditor seem to be more self explaining, than the above.

















Initial The Netspaces configurations
They are precised in the line:
var blnLoad3dWorldsOnStart=true;var blnTextEditorOnStart=true;var blnComputerTextDivMode=true;var lstTextScreenSize=[66,36];var strTextEditorCommandLineModeArgs="-maxi";/*var blnComputerTextDivMode=false;*//*var blnComputerTextDivMode=true;*//*var lstTextScreenSize=[33,18];*//*var lstTextScreenSize=[66,36];*//*var lstTextScreenSize=[132,72];*//*var strTextEditorCommandLineModeArgs="";*//*var strTextEditorCommandLineModeArgs="-mini";*//*var strTextEditorCommandLineModeArgs="";*/
with other possible options in the comments.
The line
		<meta charset="utf-8">
can be deleted - the basic functionality should work correct as it uses the basic ASCII chars. It is used to precise encoding. Without that line the national characters can be displayed incorrectly, depending on the webbrowser, weather it will recognize the encoding automatically or not. With that line there should be no problem with the html files, naturally only if the operating system and the webbrowser in question supports the utf-8 encoding, which should go smooth in most modern solutions nowadays.

Computer screen modes

The Netspaces main user interface

Shader thinkpad

Shader thinkpad is very integrated with the texteditor program, but is is more than a program - in fact it is formulated on the computer level and we describe it more on the level of the keyboard class. It is used to render the OpenGL shaders explicitely onto the screen and mix it also with the text mode. It is a great base for other such applications, not only for the OpenGL shaders. Computer text screen opacity can be set up with The Netspaces main user interface, from the Shader thinkpad section in the Extensions manager section:
Extensions manager...->Shader thinkpad...
If you don't have the "Shader thinkpad..." button you must load it first with the "Load shader thinkpad" button.
So
Extensions manager...->Shader thinkpad...->fltComputerScreenOpacity=
and enter desired value from 0.0 to 1.0 in the text field.
In order to hide and show quickly the text you can use F10 button on the keyboard (not The Netspaces main user interface F10 button) instead. Alternatively set and unset the "Show computer text screen text (F10)" checkbox in the Shader thinkpad section in the extension manager.

Graphical environment

You can run the graphical environment from the commandline by writing "startx" in the commandline. Note that you can't use the graphical environment in The Netspaces text screen mode - you can use the graphical environment in the normal The Netspaces mode and in the Computer Screen Canvas mode. When you type "startx" into the command line the wallpaper will be set to the selected quick texture repeated on the whole screen. When you right click on the wallpaper new terminal appears. You can resize the window, make it full screen, close it. In the actual version there is no taskbar. With the mouse you can resize the window by dragging (pressing once with the left mouse button and moving the mouse until desired size is set and then releasing the button) the window border. When  you will drag the title bar you will move the window. You can also move it with keyboard: "alt+arrows" will resize the window and the underlying terminal program.





Problems

We are trying to adjust the software for possibly broad range of werbbrowers. We are using possible universal instructions. Nevertheless we don't want to resign from the functionality and therefore the webbrowser must be modern enough to support the functionality we are using. In case of problems you should see at least the screen with following advice:

If you see that screen it means there must have been some error. Your browser could eventually use different standards than applied here. Newer or different browser could successfully run the code. You could also try to modify instructions from that code to instructions applied in your browser.


Discovered bugs, not solved yet

-Textwrapping error: editing one long line in the wrap mode sometimes does not align well, the cursor can go after the screen boundaries.
-Command counter not working for all commands: "x","u","ctrl+r".
-Finding and highlighting the pairing brackets.
-Indent n lines after "n>>" indents also one line before and thus n+1 lines are indented. The same also with "n<<".
-Undo, redo indenting lines does not undoes, redoes the indent, it undoes, redoes the previous operation, e.x. entering the text.
-the "read" command alias opens text file instead of inserting the external content.
-There is no way to run the freeFastCruncher command that rolls back the settings from the initFastCruncher functions with the freeFastCruncher function. Naturally you can always quickly launch that function from the extension file or the console. Anyway, such "1modeCommand" will be added in the future versions.
-Load dictionary only when InchesKeyboard is checked and dictionary has not been loaded previously.
