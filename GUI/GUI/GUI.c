#include "GUI.h"

#define BLACK "Images/black.bmp"

#define EMPTY "Images/empty.bmp"

#define WHITE "Images/white.bmp"

char player = 'B';

static SDL_Renderer *renderer;

static SDL_Texture *black_stone_texture = NULL;
static SDL_Texture *white_stone_texture = NULL;
static SDL_Texture *empty_stone_texture = NULL;

static SDL_Window *window;

int mouse_x = 0;
int mouse_y = 0;

int screen_width = 0;
int screen_height = 0;

int width;
int height;

int should_continue = 1;

char **board;

char *file_name;

int is_relevant_event(SDL_Event *event) {
    if (event == NULL) {
        return 0;
    }
    return  (event->type == SDL_MOUSEBUTTONDOWN) ||
            (event->type == SDL_KEYDOWN) ||
            (event->type == SDL_QUIT);
}

int change_vertical(int i, int j){
    int is_change_up = 0;
    int is_change_down = 0;
    int total_jumps = 0;
    char temp_array[height][width];

    //Copy all the values of board into temp_array, because we need to check vertical
    for (int k = 0; k < height; ++k) {
        for (int l = 0; l < width; ++l) {
            temp_array[k][l] = board[k][l];
        }
    }

    //Look up
    for (int k = i - 1; k >= 0; --k) {
        if(temp_array[k][j] == player) {
            if (total_jumps >= 1) {
                is_change_up = 1;
            }
            break;
        }else if(temp_array[k][j] == 'X'){
            break;
        }else {
            temp_array[k][j] = player;
            total_jumps++;
        }
    }

    if(is_change_up){ //Copy the values to the actual array
        for (int k = 0; k < height; ++k) {
            for (int l = 0; l < width; ++l) {
                board[k][l] = temp_array[k][l];
            }
        }
    }else{ //rest the temp_array
        for (int k = 0; k < height; ++k) {
            for (int l = 0; l < width; ++l) {
                temp_array[k][l] = board[k][l];
            }
        }
    }

    total_jumps = 0; //Reset the total_jumps for the down check

    //look Down
    for (int k = i + 1; k < height; ++k) {
        if(temp_array[k][j] == player){
            if(total_jumps >= 1){
                is_change_down = 1;
            }
            break;
        }else if(temp_array[k][j] == 'X'){
            break;
        }else{
            temp_array[k][j] = player;
            total_jumps++;
        }
    }

    if(is_change_down){
        for (int k = 0; k < height; ++k) {
            for (int l = 0; l < width; ++l) {
                board[k][l] = temp_array[k][l];
            }
        }
    }

    return is_change_up || is_change_down;
}

int change_diagonal(int i, int j) {
    char temp_array[height][width];

    for (int k = 0; k < height; ++k) { //fill the temp_array
        for (int l = 0; l < width; ++l) {
            temp_array[k][l] = board[k][l];
        }
    }

    int is_change_up_left = 0;
    int is_change_up_right = 0;
    int is_change_down_left = 0;
    int is_change_down_right = 0;

    // Look up-left
    int total_jumps = 0;
    for (int k = i - 1, l = j - 1; k >= 0 && l >= 0; --k, --l) {
        if (temp_array[k][l] == player) {
            if (total_jumps >= 1) {
                is_change_up_left = 1;
            }
            break;
        } else if (temp_array[k][l] == 'X') {
            break;
        } else {
            temp_array[k][l] = player;
            total_jumps++;
        }
    }

    if(is_change_up_left){
        for (int k = 0; k < height; ++k) {
            for (int l = 0; l < width; ++l) {
                board[k][l] = temp_array[k][l];
            }
        }
    }else{
        for (int k = 0; k < height; ++k) { //fill the temp_array
            for (int l = 0; l < width; ++l) {
                temp_array[k][l] = board[k][l];
            }
        }
    }

    total_jumps = 0; //Reset the total_jumps for the next check

    // Look up-right
    total_jumps = 0;  // Initialize total_jumps only once
    for (int k = i - 1, l = j + 1; k >= 0 && l < width; --k, ++l) {
        if (temp_array[k][l] == player) {
            if (total_jumps >= 1) {
                is_change_up_right = 1;
            }
            break;
        } else if (temp_array[k][l] == 'X') {
            break;
        } else {
            temp_array[k][l] = player;
            total_jumps++;
        }
    }

    if(is_change_up_right){
        for (int k = 0; k < height; ++k) {
            for (int l = 0; l < width; ++l) {
                board[k][l] = temp_array[k][l];
            }
        }
    }else{
        for (int k = 0; k < height; ++k) { //fill the temp_array
            for (int l = 0; l < width; ++l) {
                temp_array[k][l] = board[k][l];
            }
        }
    }

    total_jumps = 0; //Reset the total_jumps for the next check

    // Look down-left
    total_jumps = 0;  // Initialize total_jumps only once
    for (int k = i + 1, l = j - 1; k < height && l >= 0; ++k, --l) {
        if (temp_array[k][l] == player) {
            if (total_jumps >= 1) {
                is_change_down_left = 1;
            }
            break;
        } else if (temp_array[k][l] == 'X') {
            break;
        } else {
            temp_array[k][l] = player;
            total_jumps++;
        }
    }

    if(is_change_down_left){
        for (int k = 0; k < height; ++k) {
            for (int l = 0; l < width; ++l) {
                board[k][l] = temp_array[k][l];
            }
        }
    }else{
        for (int k = 0; k < height; ++k) { //fill the temp_array
            for (int l = 0; l < width; ++l) {
                temp_array[k][l] = board[k][l];
            }
        }
    }

    total_jumps = 0; //Reset the total_jumps for the next check

    // Look down-right
    total_jumps = 0;  // Initialize total_jumps only once
    for (int k = i + 1, l = j + 1; k < height && l < width; ++k, ++l) {
        if (temp_array[k][l] == player) {
            if (total_jumps >= 1) {
                is_change_down_right = 1;
            }
            break;
        } else if (temp_array[k][l] == 'X') {
            break;
        } else {
            temp_array[k][l] = player;
            total_jumps++;
        }
    }

    if(is_change_down_right){
        for (int k = 0; k < height; ++k) {
            for (int l = 0; l < width; ++l) {
                board[k][l] = temp_array[k][l];
            }
        }
    }else{
        for (int k = 0; k < height; ++k) { //fill the temp_array
            for (int l = 0; l < width; ++l) {
                temp_array[k][l] = board[k][l];
            }
        }
    }

    return is_change_up_left || is_change_up_right || is_change_down_left || is_change_down_right;
}

int change_horizontal(int i, int j){ //Looking horizontal in the array to change the stones needed, if there is no valid stone change horizontal you return 0
    int is_change_right = 0;
    int is_change_left = 0;
    char temp_array[width];

    int total_jumps = 0;

    for(int ii = 0; ii < width; ii++){ //Copy the values of board[i] into temp_array
        temp_array[ii] = board[i][ii];
    }

    //Look right
    for (int k = j + 1; k < width ; ++k) {
        if(temp_array[k] == player) {
            //stop, because we encountered a stone of us
            if (total_jumps >= 1) {
                is_change_right = 1;
                total_jumps = 0;
            }
            break;

        }else if(temp_array[k] == 'X'){
            break;
        }else{
            temp_array[k] = player;
            total_jumps++;
        }
    }

    if(!is_change_right){
        for(int ii = 0; ii < width; ++ii){ //Copy the values of board[i] into temp_array
            temp_array[ii] = board[i][ii];
        }
    }else{
        for (int k = 0; k < width; ++k) {
            board[i][k] = temp_array[k];
        }
    }

    total_jumps = 0; //Reset the total_jumps for the next check

    //Look left
    for (int k = j - 1; k >= 0 ; --k) {
        if(temp_array[k] == player){
            //stop
            if(total_jumps >= 1){
                is_change_left = 1;
            }
            break;
        }else if(temp_array[k] == 'X'){
            break;
        }else{
            temp_array[k] = player;
            total_jumps++;
        }
    }

    if(is_change_left){
        for (int k = 0; k < width; ++k) {
            board[i][k] = temp_array[k];
        }
    }

    return is_change_left || is_change_right;
}

int is_surrounded(int i, int j){
    for(int ii = j - 1; ii < (j - 1) + 3; ii++){
        for(int jj = i - 1; jj < (i - 1) + 3; jj++){
            if((ii >= 0) && (ii < width) && (jj >= 0) && (jj < height)){ //You can not look outside the array
                if(board[ii][jj] != 'X'){ // If there is a stone next to it
                    return 1;
                }
            }
        }
    }
    return 0;
}

int is_stone(int x, int y){ //Looks if the position clicked contains a stone
    if(board[y / 50][x / 50] == 'W' || board[y / 50][x / 50] == 'B'){
        return 1;
    }else{
        return 0;
    }
}

void save_game(const char *filename) { //If the user gave a file as input it will safe it there, else it will save it in files/game.txt
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        SDL_Log("Error: Unable to open the file.\n");
        return;
    }

    fprintf(file, "%d\n", height);
    fprintf(file, "%d\n", width);
    fprintf(file, "%c\n", player);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(file, "%c", board[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void place_stone(int x, int y){ //Will place a stone on the clicked position
    if(player == 'W'){
        int change_h = change_horizontal(y, x);
        int change_v = change_vertical(y, x);
        int change_d = change_diagonal(y, x);

        if(change_h || change_v || change_d){
            board[y][x] = player;
            save_game(file_name);
            player = 'B';
        }

    }else{
        int change_h = change_horizontal(y, x);
        int change_v = change_vertical(y, x);
        int change_d = change_diagonal(y, x);

        if(change_h || change_v || change_d){
            board[y][x] = player;
            save_game(file_name);
            player = 'W';
        }
    }
}

int is_legal_move(int x, int y){
    if(!is_stone(x, y)){ // There is no stone at that position and is it surrounded by one or more stones
       if(is_surrounded((x / 50), (y / 50))){  //We give the value of x and y as the indexes in the array of the board
           return 1;
       }
       return 0;

    }else{
        return 0;
    }
}

void read_input() {
    SDL_Event event;

    while(!SDL_PollEvent(&event) || !is_relevant_event(&event)) {}
    switch (event.type) {
        case SDL_QUIT:
            should_continue = 0;
            break;

        case SDL_MOUSEBUTTONDOWN:
            mouse_x = (event.button.x - (event.button.x % 50));
            mouse_y = (event.button.y - (event.button.y % 50));

            if(is_legal_move(mouse_x, mouse_y)){ //Is it a legal move?
                place_stone(mouse_x / 50, mouse_y / 50);
                break;
            }
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_o:
                    if(player == 'W'){ //Change player (Current player wants to skip his turn)
                        player = 'B';
                    }else{
                        player = 'W';
                    }
                    break;
                case SDLK_q:
                    should_continue = 0; //Player wants to end the game
                    break;
            }
            break;
    }
}

void clear_window() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void draw_rectangle(SDL_Texture *image, int x, int y, int width, int height) {
    SDL_Rect rectangle = { x, y, width, height };
    SDL_RenderCopy(renderer, image, NULL, &rectangle);
}

void initialize_window(const char *title) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Error while initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(title, 0, 0, screen_width, screen_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Log("Error: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
}


void initialize_textures() {
    SDL_Surface *black_stone_surface = SDL_LoadBMP(BLACK);
    SDL_Surface *white_stone_surface = SDL_LoadBMP(WHITE);
    SDL_Surface *empty_stone_surface = SDL_LoadBMP(EMPTY);

    black_stone_texture = SDL_CreateTextureFromSurface(renderer, black_stone_surface);
    white_stone_texture = SDL_CreateTextureFromSurface(renderer, white_stone_surface);
    empty_stone_texture = SDL_CreateTextureFromSurface(renderer, empty_stone_surface);

    SDL_FreeSurface(black_stone_surface);
    SDL_FreeSurface(white_stone_surface);
    SDL_FreeSurface(empty_stone_surface);
}

void draw_board(){

    int x_pos = 0;
    int y_pos = 0;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if(board[i][j] == 'X'){
                draw_rectangle(empty_stone_texture, x_pos, y_pos, 50, 50);
            }else if(board[i][j] == 'B'){
                draw_rectangle(black_stone_texture, x_pos, y_pos, 50, 50);
            }else if(board[i][j] == 'W'){
                draw_rectangle(white_stone_texture, x_pos, y_pos, 50, 50);
            }
            x_pos += 50;
        }
        x_pos = 0;
        y_pos += 50;
    }
}

void draw_window() {
    clear_window();
    draw_board();
    SDL_RenderPresent(renderer);
}

void initialize_gui() {
    initialize_window("Othello");
    SDL_SetWindowPosition(window, 800, 100);
    initialize_textures();
}

void free_board(){
    // Free the allocated memory for the game board
    for (int i = 0; i < height; i++) {
        free(board[i]);
    }
    free(board);
}

void free_gui() {
    SDL_DestroyTexture(black_stone_texture);
    SDL_DestroyTexture(empty_stone_texture);
    SDL_DestroyTexture(white_stone_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void initialize_board(){ //Width and height are global variables
    board = (char **)malloc(height * sizeof(char *)); // Allocate memory for the rows

    for (int i = 0; i < height; i++) { // Allocate memory for each row
        board[i] = (char *)malloc(width * sizeof(char));
    }
}

void read_game_file(FILE *file) {
    if (fscanf(file, " %d %d %c", &height, &width, &player) != 3) {
        // Failed to read width, height, and player
        SDL_Log("Error: Unable to read game parameters from the file.\n");
        fclose(file);
        return;
    }

    initialize_board();

    //set the screen pixels
    screen_height = height * 50;
    screen_width = width * 50;

    // Read the game board
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (fscanf(file, " %c", &board[i][j]) != 1) {
                // Failed to read a character from the board
                SDL_Log("Error: Unable to read the game board from the file.\n");
                fclose(file);
                return;
            }
        }
    }
}

int can_change_horizontal(int i, int j) {
    char temp_array[width];

    for (int k = 0; k < width; ++k) {
        temp_array[k] = board[i][k];
    }

    // Check right
    for (int k = j + 1; k < width; ++k) {
        if (temp_array[k] == player) {
            return 1;
        } else if (temp_array[k] == 'X') {
            break;
        }
    }

    // Check left
    for (int k = j - 1; k >= 0; --k) {
        if (temp_array[k] == player) {
            return 1;
        } else if (temp_array[k] == 'X') {
            break;
        }
    }

    return 0;
}

int can_change_vertical(int i, int j) {
    char temp_array[height];

    for (int k = 0; k < height; ++k) {
        temp_array[k] = board[k][j];
    }

    // Check up
    for (int k = i - 1; k >= 0; --k) {
        if (temp_array[k] == player) {
            return 1;
        } else if (temp_array[k] == 'X') {
            break;
        }
    }

    // Check down
    for (int k = i + 1; k < height; ++k) {
        if (temp_array[k] == player) {
            return 1;
        } else if (temp_array[k] == 'X') {
            break;
        }
    }

    return 0;
}

int can_change_diagonal(int i, int j) {
    char temp_array[height][width];

    for (int k = 0; k < height; ++k) {
        for (int l = 0; l < width; ++l) {
            temp_array[k][l] = board[k][l];
        }
    }

    // Check up-left
    for (int k = i - 1, l = j - 1; k >= 0 && l >= 0; --k, --l) {
        if (temp_array[k][l] == player) {
            return 1;
        } else if (temp_array[k][l] == 'X') {
            break;
        }
    }

    // Check up-right
    for (int k = i - 1, l = j + 1; k >= 0 && l < width; --k, ++l) {
        if (temp_array[k][l] == player) {
            return 1;
        } else if (temp_array[k][l] == 'X') {
            break;
        }
    }

    // Check down-left
    for (int k = i + 1, l = j - 1; k < height && l >= 0; ++k, --l) {
        if (temp_array[k][l] == player) {
            return 1;
        } else if (temp_array[k][l] == 'X') {
            break;
        }
    }

    // Check down-right
    for (int k = i + 1, l = j + 1; k < height && l < width; ++k, ++l) {
        if (temp_array[k][l] == player) {
            return 1;
        } else if (temp_array[k][l] == 'X') {
            break;
        }
    }

    return 0;
}

void fill_board() {
    initialize_board();

    if(board == NULL){
        SDL_Log("Error filling the board");
    }

    char p = 'W';

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if ((i == height / 2 - 1 && j == width / 2 - 1) || (i == height / 2 && j == width / 2 - 1)) { // Fill with white and black
                board[i][j] = p;
                if (p == 'W') {
                    p = 'B';
                } else {
                    p = 'W';
                }
                board[i][j + 1] = p;
                j++;
            } else {
                board[i][j] = 'X';
            }
        }
    }

    screen_width = width * 50;
    screen_height = height * 50;
}

void make_game_file(){
    file_name = "files/game.txt";
}

void determine_winner() {
    int black_count = 0;
    int white_count = 0;

    // Count the number of black and white stones on the board
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (board[i][j] == 'B') {
                black_count++;
            } else if (board[i][j] == 'W') {
                white_count++;
            }
        }
    }

    // Determine the winner based on the stone counts
    if (black_count > white_count) {
        SDL_Log("Black player wins!\n");
    } else if (black_count < white_count) {
        SDL_Log("White player wins!\n");
    } else {
        SDL_Log("It's a tie!\n");
    }
}

int has_legal_moves() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (board[i][j] == 'X' && (can_change_diagonal(i, j) || can_change_horizontal(i, j) || can_change_vertical(i, j))) {
                return 1;
            }
        }
    }
    return 0;
}

void free_memory(){
    SDL_Log("Freeing Memory\n");
    free_board();
    free_gui();
}

int main(int argc, char *argv[]) {

    int fill = 0;

    if(argc > 1){ //Does it have 1 or more arguments?
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-' && i + 1 < argc) {  // is it an option flag
                if (argv[i][1] == 'w') {
                    width = atoi(argv[i + 1]);  // Convert the value to integer
                    fill = 1;
                } else if (argv[i][1] == 'h') {
                    height = atoi(argv[i + 1]);  // Convert the value to an integer and fill the board
                    fill = 1;
                    make_game_file();
                } else if (argv[i][1] == 'f') {
                    file_name = argv[i + 1];
                    read_game_file(fopen(file_name, "r"));
                }
            }
        }
    }else{ //If not, fill with default
        width = 8; //The default width
        height = 8; //The default height
        fill = 1;
        make_game_file(); //set file name to game.txt
    }

    if(fill){
        fill_board();
    }

    initialize_gui();

    save_game(file_name);

    while (should_continue) {
        draw_window();
        read_input();

        if (!has_legal_moves()) {
            determine_winner();
            should_continue = 0;
        }
    }

    /* Dealloceert al het geheugen dat werd aangemaakt door SDL zelf. */
    free_memory();

    return 0;
}