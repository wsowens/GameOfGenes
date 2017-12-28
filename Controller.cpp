#include "Controller.h"
#include <exception>
#include <sstream>

Controller::Controller(SDL_Window * window)
{
	std::cerr << "controller constructor called\n";
	if (window == NULL)
	{
		std::cerr << "WINDOW IS NULL";
	}
	this->mainRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if( mainRenderer == NULL )
	{
		std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
	}
    //Set default speed to 25
    speed = 25;

	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	std::cerr << windowWidth << " height: " << windowHeight << std::endl;


	std::cerr << "Board pointer: " << this->boardPanel << std::endl;

	this->boardPanel->x = 0;
	this->boardPanel->y = 0;
	this->boardPanel->w = windowWidth;
	this->boardPanel->h = windowHeight * 5 / 6;

	std::cerr << "Board panel: " << boardPanel->w << " " << boardPanel->h << std::endl;


	this->statusPanel->x = 0;
	this->statusPanel->y = windowHeight * 5 / 6;
	this->statusPanel->w = windowWidth;
	this->statusPanel->h = windowHeight / 6;

	std::cerr << "Status panel: " << this->statusPanel->w << " " << this->statusPanel->h << std::endl;
	std::cerr << "Status pointer: " << this->statusPanel << std::endl;
	this->event = new SDL_Event();
	this->mainColor = {0, 0xFF, 0};
	this->bgColor = {0, 0, 0};
	this->accentColor = {0xFF, 0xFF, 0xFF};
    state = menu;
	this->mainFont = TTF_OpenFont("./assets/consola.ttf", 16);

}

void Controller::printPanelDimensions()
{
	std::cerr << "Print method called\n";
	std::cerr << "Board panel: " << boardPanel->w << " " << boardPanel->h << std::endl;
	std::cerr << "Board pointer: " << this->boardPanel << std::endl;
	std::cerr << "Status panel: " << this->statusPanel->w << " " << this->statusPanel->h << std::endl;
	std::cerr << "Status pointer: " << this->statusPanel << std::endl;
}
/*Postconditions: updates the terminal screen
*/
void Controller::updateScreen()
{
	std::cerr << "updating screen\n";
	SDL_RenderPresent(mainRenderer);
}

/*Preconditions: wrapAround is true if the user wants to enable wrapAround
                and false if the user does not
Postconditions: creates a new board with the max possible height and width
*/
void Controller::createNewBoard(bool wrapAround)
{
	//TODO: ACTUALLY CALCULATE THE MAXIMUM?
	createNewBoard(wrapAround, 25, 40);
}

/*Preconditions: wrapAround is true if the user wants to enable wrapAround
                and false if the user does not
                height and width are integers
Postconditions: creates a new board. If height or width are <= 0, sets it
                to 1, if height or width are greater than the terminal size,
                sets them to the max possible size
*/
void Controller::createNewBoard(bool wrapAround, int height, int width)
{
    //delete the old board if necessary
	std::cerr << "creating new board\n";
    if (board != NULL)
    {
        delete board;
        board = NULL;
	    //TODO: possibly clear renderer here
    }
    //check the height and width and adjust if necessary
	//TODO: is this still necessary?
	/*
    width = (width > 0) ? width:1;
    width = (width <= BOARD_WIDTH-2) ? width:BOARD_WIDTH-2;
    height = (height > 0) ? height:1;
    height = (height <= BOARD_HEIGHT-2) ? height:BOARD_HEIGHT-2;
	*/
	board = new Board(wrapAround, height, width);
    SDL_RenderClear(mainRenderer);
	renderStatusPanel();
	renderBoard();
    updateScreen();
}

/*Preconditions: filename is a properly constructed string
Postconditions: Constructs a new board based on filename
                Throws an error if the loaded board is bigger than the
                max obard size
*/
/*
void Controller::createNewBoard(std::string filename)
{
    //delete previous board if necessary
    if(board != nullptr){
        delete board;
        board = nullptr;
        wclear(panel_window(boardPanel));
        wclear(panel_window(statusPanel));
    }
    //May throw an error if the file does not exist
    board = new Board("boards" + separator() + filename);
    int height = board->getHeight();
    int width = board->getWidth();
    //Throw an error if the board is bigger than the max board size
    if(height > BOARD_HEIGHT || width > BOARD_WIDTH)
    {
        throw "Board is too big!";
    }
    width = (width > 0) ? width:1;
    width = (width <= BOARD_WIDTH-2) ? width:BOARD_WIDTH-2;
    height = (height > 0) ? height:1;
    height = (height <= BOARD_HEIGHT-2) ? height:BOARD_HEIGHT-2;
    //Create a new board window to display the board
    WINDOW *boardWin = newwin(height+2, width+2, (BOARD_HEIGHT-height)/2-1, (BOARD_WIDTH-width)/2-1);
    boardPanel = new_panel(boardWin);
    box(boardWin, 0, 0);
    //Create window for the bottom status area
    WINDOW *statusWin = newwin(STATUS_HEIGHT, STATUS_WIDTH, BOARD_HEIGHT, 0);
    statusPanel = new_panel(statusWin);
    box(statusWin, 0, 0);
    updateStatusWin();
    updateScreen();
}
*/

/*Preconditions: ratio is a double >= 0
Postconditions: Randomly chooses to toggle each cell of the board, based on ratio
*/
void Controller::randomizeBoard(double ratio)
{
    board->randomize(ratio);
}

/*Postconditions: returns controller speed
*/
int Controller::getSpeed()
{
    return speed;
}

/*Postconditions: returns the current save status of the board
*/
bool Controller::isSaved()
{
    return board->getIsSaved();
}

/*Postconditions: returns controller state
*/
controlState Controller::getState()
{
    return state;
}

/*Postconditions: returns controller state in string form
*/
std::string Controller::getStateName()
{
    switch(state)
    {
        case running:
            return "Running";
        case paused:
            return "Paused";
        case editing:
            return "Editing";
        case menu:
            return "Menu";
        case exiting:
            return "Exiting";
    }
    return "MENU";
}

/*Preconditions: newState is one of the control states delcared in controller.h
Postconditions: sets the controller state to newState
*/
void Controller::setState(controlState newState)
{
    state = newState;
    if(state != exiting)
        renderStatusPanel(statusPanel);
}

/*Preconditions: newSpeed is -1 or 1
Postconditions: increases or decreases the controller speed, based on the value
                of newSpeed and the current speed
*/
void Controller::setSpeed(int newSpeed)
{
    /*adds the value to the current value of speed. Prevents users from
    decreasing below 1, and exceeding 10000
    scaling the increment depending on the size of the speed (to allow users
    to scroll at a reasonable rate) */
    if (speed >= 10000)
    {
        newSpeed *= 1000;
    }

    else if (speed >= 10000) {
        newSpeed *= 1000;
    }
    else if (speed >= 1000)
    {
        newSpeed *= 100;
    }
    else if (speed >= 100)
    {
        newSpeed *= 10;
    }
    //make sure the speed is >= 1
    if (speed+newSpeed < 1)
    {
        speed = 1;
        return;
    }
    //make sure speed does not exceed 100000
    if (speed+newSpeed > 100000)
    {
        speed = 100000;
        return;
    }
    speed += newSpeed;
}

/*Postconditions: Updates the status windows, based on the current values of
                height, width, state, iterations, birth, death, and speed
*/
/*
void Controller::updateStatusWin()
{
    WINDOW* statusWin = panel_window(statusPanel);
    werase(statusWin);
    box(statusWin, 0, 0);
    if (state == editing)
        mvwprintw(statusWin, 1, 1, "Board Size:\tStatus:");
    else
        mvwprintw(statusWin, 1, 1, "Board Size:\tStatus:\tIterations:\tBirths:\tDeaths:\tSpeed:\tBirth Rule:\tSurvival Rule:");
    //pad the board size when it is small
    mvwprintw(statusWin, 3, 1, "%-3d", board->getHeight());
    wprintw(statusWin, " x ");
    wprintw(statusWin, "%-3d", board->getWidth());
    wprintw(statusWin, "\t");
    wprintw(statusWin, getStateName().c_str());
    wprintw(statusWin, "\t");
    if (state != editing)
    {
        wprintw(statusWin, "%d", board->getIterations());
        wprintw(statusWin, "\t\t");
        wprintw(statusWin, "%d", board->getBirths());
        wprintw(statusWin, "\t");
        wprintw(statusWin, "%d", board->getDeaths());
        wprintw(statusWin, "\t");

        //speed variable is in iterations/second... make sure it is presented that
        //way to the user (i.e. don't divide by 1000 or anything)
        wprintw(statusWin, "%d", speed);

        wprintw(statusWin, "\t");
        wprintw(statusWin, "%-9s", board->getBirthRule().c_str());
        wprintw(statusWin, "\t");
        wprintw(statusWin, "%-9s", board->getSurvivalRule().c_str());
    }
    mvwprintw(statusWin, 2, STATUS_WIDTH-15, "K: Keybindings");
    updateScreen();
} */
/*Preconditions: message is a string object
Postconditions: Prompts the user with message, and gets string input from the user
*/
std::string Controller::getStringInput(std::string message)
{
	std::cout << message;
	std::string userInput;
	std::cin >> userInput;
    return userInput;
}

/*
void Controller::getRules()
{
    //Show the cursor
    curs_set(TRUE);
    //Create the fields and configure them
    FIELD *field[5];
	int rows, cols;
	field[0] = new_field(1, 20, 2, 0, 0, 0);
    field[1] = new_field(1, 10, 2, 22, 0, 0);
    field[2] = new_field(1, 20, 3, 0, 0, 0);
    field[3] = new_field(1, 10, 3, 22, 0, 0);
	field[4] = NULL;
    field_opts_off(field[0], O_ACTIVE);
    set_field_buffer(field[0], 0, "Birth Rule: ");
	set_field_back(field[1], A_UNDERLINE);
	field_opts_off(field[1], O_AUTOSKIP);
    set_field_type(field[1], TYPE_INTEGER, 0, 0, 123456789);
    field_opts_off(field[2], O_ACTIVE);
    set_field_buffer(field[2], 0, "Survival Rule: ");
    set_field_back(field[3], A_UNDERLINE);
	field_opts_off(field[3], O_AUTOSKIP);
    set_field_type(field[3], TYPE_INTEGER, 0, 0, 123456789);
    //Create the form
    FORM *form = new_form(field);
    scale_form(form, &rows, &cols);
    WINDOW *formWin = newwin(rows + 4, cols + 4, termRow / 2 - (rows + 4) / 2, termCol / 2 - cols / 2);
    PANEL *formPanel = new_panel(formWin);
    keypad(formWin, TRUE);
    set_form_win(form, formWin);
    WINDOW* subFormWin = derwin(formWin, rows, cols, 2, 2);
    set_form_sub(form, subFormWin);
    box(formWin, 0, 0);
    printCenter(formWin, "Enter rules:", 1, cols + 4);
    //display the form
    post_form(form);
    show_panel(formPanel);
    updateScreen();
    set<int> birthTemp, survivalTemp;
    bool birthSelected = true;
    wchar_t ch;
	int value = 0;
    //Loop until the user enters valid values for both fields and presses enter
    while((ch = wgetch(formWin)))
    {
        switch(ch)
        {
            //Switch fields
            case KEY_UP:
            case KEY_DOWN:
            case '\t':
                form_driver(form, REQ_NEXT_FIELD );
                birthSelected = birthSelected ? false : true;
                birthTemp = rule2set(string(field_buffer(field[1], 0)));
                survivalTemp = rule2set(string(field_buffer(field[3], 0)));
                break;
            case KEY_LEFT:
				form_driver(form, REQ_PREV_CHAR);
				break;
            case KEY_RIGHT:
				form_driver(form, REQ_NEXT_CHAR);
				break;
            //Delete the character before the cursor
            case '\b':
            case KEY_BACKSPACE:
                form_driver(form, REQ_PREV_CHAR);

                //remove deleted character from set
                if(birthSelected)
					birthTemp.erase((char)winch(subFormWin) - '0');
				else
					survivalTemp.erase((char)winch(subFormWin) - '0');

                form_driver(form, REQ_DEL_CHAR);
                break;

            //Delete the character at the cursor
            case KEY_DC:
                //remove deleted character from set
                if(birthSelected)
					birthTemp.erase((char)winch(subFormWin) - '0');
				else
					survivalTemp.erase((char)winch(subFormWin) - '0');

                form_driver(form, REQ_DEL_CHAR);
                break;

            //Check the values of the fields
            //If one is not filled, use the enter key to switch fields
            case 10:
                form_driver(form, REQ_VALIDATION);
                //set the new rules
                board->setBirthRule(rule2set(string(field_buffer(field[1], 0))));
                board->setSurvivalRule(rule2set(string(field_buffer(field[3], 0))));
                //delete the form
                curs_set(FALSE);
                unpost_form(form);
                free_form(form);
                for(int i = 0; i < 4; ++i)
                {
                    free_field(field[i]);
                }
                hide_panel(formPanel);
                delwin(formWin);
                del_panel(formPanel);
                updateStatusWin();
                updateScreen();
                return;
            case 27:
				return;
            //Add the character to the field
            default:
                form_driver(form, REQ_VALIDATION);
				value = atoi((char*)&ch);
						form_driver(form, REQ_END_FIELD);
				if(birthSelected)
				{
					birthTemp = rule2set(string(field_buffer(field[1], 0)));
					if(birthTemp.find(value) == birthTemp.end())
					{
						form_driver(form, ch);
						birthTemp = rule2set(string(field_buffer(field[1], 0)));

					}
				}
				else
				{
					survivalTemp = rule2set(string(field_buffer(field[3], 0)));
					if(survivalTemp.find(value) == survivalTemp.end())
					{
						form_driver(form, ch);
						survivalTemp = rule2set(string(field_buffer(field[3], 0)));
					}
				}
                break;
        }
    }
}
*/

/*Postconditions: displays the board in the board window
*/
//TODO: delete this
/*
void Controller::printBoard()
{
    std::vector<std::vector<bool>> matrix = board->getMatrix();
    for (int r = 0; r < board->getHeight(); r++)
    {
        for (int c=0; c < board->getWidth(); c++)
        {
            if(matrix[r][c])
                waddch(win, 'X');
            else
                waddch(win, ' ');

        }
        //move to the next row
        wmove(win, r+2, 1);
    }
    show_panel(boardPanel);
    updateScreen();
    //move cursor back to its original position
    wmove(win, y, x);
}
*/
void Controller::renderBoard(SDL_Rect * renderArea)
{
	std::cerr << "render board called\n";
	SDL_Color * cellColor = NULL;
	printPanelDimensions();
	std::vector<std::vector<bool>> matrix = board->getMatrix();

	int cellWidth = renderArea->w / this->board->getWidth();
	int cellHeight = renderArea->h / this->board->getHeight();
	for (int row = 0; row < this->board->getHeight(); row++)
	{
		for (int column = 0; column < this->board->getWidth(); column++)
		{
			int y = renderArea->y + cellHeight * row;
			int x = renderArea->x + cellWidth * column;
			SDL_Rect cellRect = {x, y, cellWidth, cellHeight};

			cellColor = &this->mainColor;
			//TODO: make this render a nice box around the cell instead of just changing the color
			if (this->currentRow == row && this->currentCol == column)
			{
				SDL_SetRenderDrawColor(mainRenderer, accentColor.r, accentColor.g, accentColor.b, 0xFF);
				SDL_RenderFillRect(mainRenderer, &cellRect);
			}
			if (!matrix[row][column])
			{
				cellColor = &this->bgColor;
			}
			SDL_SetRenderDrawColor(mainRenderer, cellColor->r, cellColor->g, cellColor->b, cellColor->a);
			bool filled = false;
			if (!filled)
			{
				cellRect.x++;
				cellRect.y++;
				cellRect.w -= 2;
				cellRect.h -= 2;
			}
			SDL_RenderFillRect(mainRenderer, &cellRect);
		}
	}
	SDL_SetRenderDrawColor(mainRenderer, bgColor.r, bgColor.g, bgColor.b, 0xFF);
}

void Controller::renderBoard()
{
	SDL_Point temp = {-1, -1};
	renderBoard(boardPanel);
}

SDL_Point Controller::getCell(int x, int y)
{
	//TODO: obviously, make the cell height a data field... this will let you control zoom
	int cellWidth = boardPanel->w / this->board->getWidth();
	int cellHeight = boardPanel->h / this->board->getHeight();
	this->currentCol = (x - boardPanel->x) / cellWidth;
	this->currentRow = (y - boardPanel->y) / cellHeight;
	return {currentRow, currentCol};
}

void Controller::renderStatusPanel(SDL_Rect * renderArea)
{
	std::vector<std::string> stringList;
	std::ostringstream boardSize, status;
	//creating string streams for board size and status
	boardSize << "Size: " << board->getHeight() << "x" << board->getWidth();
	status << "Status: " << getStateName();
	//adding strings to stringList
	stringList.push_back(boardSize.str());
	stringList.push_back(status.str());

	//if we are not editing, we can also add these  details
	if (state != editing)
	{
		std::ostringstream iterations, births, deaths, speed;
		iterations << "Iterations: " << board->getIterations();
		births << "Births: " << board->getBirths();
		deaths << "Deaths: " << board->getDeaths();
		speed << "Speed: " << this->speed;
		stringList.push_back(iterations.str());
		stringList.push_back(births.str());
		stringList.push_back(deaths.str());
		stringList.push_back(speed.str());
	}

	//finding an even amount of space along each dimension
	int yShift = renderArea->h / 3;
	int xShift = renderArea->w / 4;

	for (int i = 0; i < stringList.size(); i++)
	{
		int row = i / 3 + 1;
		int col = i % 3 + 1;

		SDL_Surface * surface = TTF_RenderText_Shaded(mainFont, stringList[i].c_str(), mainColor, bgColor);
		//centering each surface onto a point in the 2x3 grid
		SDL_Rect tempRect = {renderArea->x + xShift * col - surface->w/2, renderArea->y + yShift * row - surface->h/2, surface->w, surface->h};
		//creating a texture from surface and rendering it onto tempRect
		SDL_RenderCopy(mainRenderer, SDL_CreateTextureFromSurface(mainRenderer, surface), NULL, &tempRect);
	}
	SDL_SetRenderDrawColor(mainRenderer, mainColor.r, mainColor.g, mainColor.b, 0xFF);
	SDL_RenderDrawLine(mainRenderer, 0, renderArea->y, renderArea->w, renderArea->y);
	SDL_SetRenderDrawColor(mainRenderer, bgColor.r, bgColor.g, bgColor.b, 0xFF);
}

void Controller::renderStatusPanel()
{
	renderStatusPanel(statusPanel);
}

void Controller::runIteration()
{
    board->runIteration();
}

int Controller::getButtonInput(std::string dialog, std::vector<std::string> options)
{
	std::cerr << "get button input\n";
	ButtonBox buttons(mainFont, mainColor, bgColor, dialog, options, boardPanel->w, boardPanel->h, true);

	std::cerr << "buttons created\n";
	buttons.render(mainRenderer);
	std::cerr << "buttons rendered\n";
	SDL_RenderPresent(mainRenderer);
	std::cerr << "render presented\n";
	while (!buttons.hasValidInput())
	{
		while (SDL_PollEvent(event) != 0)
		{
			if (event->type == SDL_QUIT)
			{
				exit(0); //see ya
			}
			else if (event->type == SDL_MOUSEBUTTONDOWN)
			{
				int a, b;
				SDL_GetMouseState(&a, &b);
				buttons.updateInput(a, b);
			}
			else if (event->type == SDL_MOUSEMOTION)
			{
				int a, b;
				SDL_GetMouseState(&a, &b);
				buttons.updateHover(a, b);
			}
		}
		buttons.render(mainRenderer);
		SDL_RenderPresent(mainRenderer);
	}
	//TODO:delete buttons?
	return buttons.getInput();
}
bool Controller::getYesOrNo(std::string dialog)
{
	std::vector<std::string> options;
	options.push_back("Yes");
	options.push_back("No");
	//the exclamation point is because Yes corresponds to 0, which is false
	//intuitively, yes should correspond to "true";
	return !getButtonInput(dialog, options);
}

/*Preconditions: dialog is a string
Postconditions: displays dialog to the user, and wait for a keypress
*/
void Controller::confirmationBox(std::string dialog)
{
	std::vector<std::string> options;
	options.push_back("Ok.");
	ButtonBox buttons(mainFont, mainColor, mainColor, dialog, options, boardPanel->w / 2, boardPanel->h / 2, true);
	buttons.render(mainRenderer);
	SDL_RenderPresent(mainRenderer);
	while (!buttons.hasValidInput())
	{
		while (SDL_PollEvent(event) != 0)
		{
			if (event->type == SDL_QUIT)
			{
				exit(0);
			}
			else if (event->type == SDL_MOUSEBUTTONDOWN)
			{
				int a, b;
				SDL_GetMouseState(&a, &b);
				buttons.updateInput(a, b);
			}
			else if (event->type == SDL_MOUSEMOTION)
			{
				int a, b;
				SDL_GetMouseState(&a, &b);
				buttons.updateHover(a, b);
			}
			//TODO: exit if enter key is pressent
		}
	}
}

//possibly not useful
/*Postconditions: displays the keybindings to the user...possibly not useful
*/
//TODO: add a help box again

void Controller::keybindingsBox()
{
	std::cout << "Help box coming soon." << std::endl;
	//create the window and print the keybindings
    /*WINDOW *dialogWin = newwin(15, 38, termRow / 2 - 7, termCol / 2 - 19);
    keypad(dialogWin, TRUE);
    PANEL *dialogPanel = new_panel(dialogWin);
    box(dialogWin, 0, 0);
    mvwprintw(dialogWin, 1, 1, "p : pause/resume");
    mvwprintw(dialogWin, 2, 1, "[ : decrease speed");
    mvwprintw(dialogWin, 3, 1, "] : increase speed");
    mvwprintw(dialogWin, 4, 1, "Arrow Keys : move cursor");
    mvwprintw(dialogWin, 5, 1, "a : add pattern");
    mvwprintw(dialogWin, 6, 1, "; : rotate pattern counterclockwise");
    mvwprintw(dialogWin, 7, 1, "' : rotate pattern clockwise");
    mvwprintw(dialogWin, 8, 1, "r : edit rules");
    show_panel(dialogPanel);
    updateScreen();
    //wait for a keypress
    wgetch(dialogWin);
    //delete the window
    curs_set(FALSE);
    hide_panel(dialogPanel);
    delwin(dialogWin);
    del_panel(dialogPanel);
    updateScreen();*/
}


/*Preconditions: height and width are references to integers
Postconditions: sets height and width equal to the user input values
*/
/*
void Controller::GetPatternDimensions(int &height, int &width)
{
    //Show the cursor
    curs_set(TRUE);
    //Create the fields and configure them
    FIELD *field[5];
	int rows, cols;
	field[0] = new_field(1, 8, 2, 0, 0, 0);
    field[1] = new_field(1, 5, 2, 8, 0, 0);
    field[2] = new_field(1, 7, 2, 15, 0, 0);
    field[3] = new_field(1, 5, 2, 22, 0, 0);
	field[4] = NULL;
    field_opts_off(field[0], O_ACTIVE);
    set_field_buffer(field[0], 0, "Height: ");
	set_field_back(field[1], A_UNDERLINE);
	field_opts_off(field[1], O_AUTOSKIP);
    set_field_type(field[1], TYPE_INTEGER, 0, 1, BOARD_HEIGHT);
    field_opts_off(field[2], O_ACTIVE);
    set_field_buffer(field[2], 0, "Width: ");
    set_field_back(field[3], A_UNDERLINE);
	field_opts_off(field[3], O_AUTOSKIP);
    set_field_type(field[3], TYPE_INTEGER, 0, 1, BOARD_WIDTH);
    //Create the form
    FORM *form = new_form(field);
    scale_form(form, &rows, &cols);
    WINDOW *formWin = newwin(rows + 4, cols + 4, termRow / 2 - (rows + 4) / 2, termCol / 2 - cols / 2);
    PANEL *formPanel = new_panel(formWin);
    keypad(formWin, TRUE);
    set_form_win(form, formWin);
    WINDOW* subFormWin = derwin(formWin, rows, cols, 2, 2);
    set_form_sub(form, subFormWin);
    box(formWin, 0, 0);
    printCenter(formWin, "Enter pattern dimensions:", 1, cols + 4);
    //display the form
    post_form(form);
    show_panel(formPanel);
    updateScreen();
    int heightInput = 0, widthInput = 0;
    wchar_t ch;
    //Loop until the user enters valid values for both fields and presses enter
    while((ch = wgetch(formWin)))
    {
        switch(ch)
        {
            //Switch fields
            case KEY_LEFT:
            case KEY_RIGHT:
            case '\t':
                form_driver(form, REQ_NEXT_FIELD);
                break;
            //Delete the character before the cursor
            case '\b':
            case KEY_BACKSPACE:
                form_driver(form, REQ_PREV_CHAR);
                form_driver(form, REQ_DEL_CHAR);
                break;
            //Delete the character at the cursor
            case KEY_DC:
                form_driver(form, REQ_DEL_CHAR);
                break;
            //Check the values of the fields
            //If one is not filled, use the enter key to switch fields
            case 10:
                form_driver(form, REQ_VALIDATION);
                heightInput = atoi( field_buffer(field[1], 0) );
                widthInput = atoi( field_buffer(field[3], 0) );
                if( (heightInput == 0) ^ (widthInput == 0) )
                {
                    form_driver(form, REQ_NEXT_FIELD);
                    break;
                }
                height = heightInput;
                width = widthInput;
                //delete the form
                curs_set(FALSE);
                unpost_form(form);
                free_form(form);
                for(int i = 0; i < 4; ++i)
                {
                    free_field(field[i]);
                }
                hide_panel(formPanel);
                delwin(formWin);
                del_panel(formPanel);
                updateScreen();
                return;

            //Add the character to the field
            default:
                form_driver(form, ch);
                break;
        }
    }
}
*/

/*Postconditions: gets a ratio input from the user
*/
double Controller::getRatioInput(std::string message)
{
	std::cout << message;
	double ratio;
	std::cin >> ratio;
    return ratio;
}

/*A sub control loop, allows the user the manually toggle cells and add patterns
*/
//TODO:add better exit(0) (i.e. actually add a method)
void Controller::editMode()
{
	int x = 0, y = 0, r;
	SDL_Point cell = getCell(x, y);
    //loop until we are no longer paused or editing
    while(getState() == paused || getState() == editing)
    {
		SDL_RenderClear(mainRenderer);
		renderBoard(this->boardPanel);
		renderStatusPanel(this->statusPanel);
		SDL_RenderPresent(mainRenderer);
		updateScreen();
		while (SDL_PollEvent(event) != 0)
		{
		    switch(this->event->type)
			{
				case SDL_QUIT:
				std::cout << "exiting...\n";
				exit(-1); //TODO: exit more gracefully
				break;
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONUP:
				SDL_GetMouseState(&x, &y);
				cell = getCell(x, y);
				break;
				case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&x, &y);
				cell = getCell(x, y);
				//TODO: get correct cell!!!
				this->board->toggle(this->currentRow, this->currentCol);
				break;
				case SDL_KEYDOWN:
				switch(this->event->key.keysym.sym)
				{
					case SDLK_UP:
					this->currentRow--;
					break;
					case SDLK_DOWN:
					this->currentRow++;
					//TODO:make sure you can't go out of bounds
					break;
					case SDLK_LEFT:
					this->currentCol--;
					break;
					case SDLK_RIGHT:
					this->currentCol++;
					break;
					case SDLK_SPACE:
					this->board->toggle(this->currentRow, this->currentCol);
					break;
					case SDLK_RETURN:
					runIteration();
					break;
					case SDLK_RIGHTBRACKET:
					setSpeed(1);
					break;
					case SDLK_LEFTBRACKET:
					setSpeed(-1);
					break;
					case SDLK_h:
					keybindingsBox();
					break;
					case SDLK_a:
					std::cout << currentRow << " " << currentCol << std::endl;
					addPattern(this->currentRow, this->currentCol);
					break;
					case SDLK_p:
					setState(running);
					break;
					case SDLK_ESCAPE:
					setState(menu);
				}
	        }
		}
	}
}

void Controller::runningMode()
{

	while(getState() == running)
    {
		//TODO: use timing to make this better
		SDL_RenderClear(mainRenderer);
		SDL_Delay(1.0/getSpeed() * 1000);
		for (int i = 1; (getSpeed () > 1000) && (i < getSpeed() / 1000); i++)
		{
			runIteration();
		}
		runIteration();
		renderBoard();
		renderStatusPanel();
		updateScreen();
		while (SDL_PollEvent(event) != 0)
		{
		    switch(this->event->type)
			{
				case SDL_QUIT:
				std::cout << "exiting...\n";
				exit(-1); //TODO: exit more gracefully
				break;
				case SDL_KEYDOWN:
				switch(this->event->key.keysym.sym)
				{
					case SDLK_UP:
					this->currentRow--;
					break;
					case SDLK_DOWN:
					this->currentRow++;
					//TODO:make sure you can't go out of bounds
					break;
					case SDLK_LEFT:
					this->currentCol--;
					break;
					case SDLK_RIGHT:
					this->currentCol++;
					break;
					case SDLK_RIGHTBRACKET:
					setSpeed(1);
					break;
					case SDLK_LEFTBRACKET:
					setSpeed(-1);
					break;
					case SDLK_h:
					keybindingsBox();
					break;
					case SDLK_p:
					setState(paused);
					break;
					case SDLK_ESCAPE:
					setState(menu);
				}
	        }
		}
	}
}

void Controller::addPattern(int startR, int startC)
{
	std::cout << "Sorry, I'll add this feature later.\n";
}
/*Preconditions: matrix is a vector of vectors of bools
Postconditions: renders matrix on top of the board
*/
//TODO: fix this method
/*
void Controller::renderPattern(std::vector<std::vector<bool>>& matrix)
{

}
*/

/*Postconditions: Saves the board if the users selects yes
                does nothing otherwise
*/
//TODO: make it happen
/*
void Controller::saveCurrent()
{
	bool shouldSave = getYesOrNo("Would you like to save?", options);
    if(shouldSave)
    {
        std::string filename = getStringInput("Enter a filename:");
        //If the state is editing, then the program is in the pattern editor
        //so the file should be saved to the pattern folder
        if(state == editing)
        {
            filename = "patterns" + separator() + filename;
        }
        //Otherwise, save it to the board folder
        else
        {
            filename = "boards" + separator() + filename;
        }
        board->saveState(filename);
    }
}
*/
