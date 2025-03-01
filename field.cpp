// This file contains all the maze generation algorithms used

#include "field.h"

Field::Field(int rows, int columns, double length, int seed)
{
    field = new Cell ** [rows];
    for (int row =0; row < rows; row++){
        field[row] = new Cell* [columns];
        for (int column =0; column < columns; column++){
            field[row][column] = new Cell(length, 15);
            field[row][column]->moveBy(length*column, length*row);
        }
    }
    this->rows = rows;
    this->columns = columns;
    this->traced = false;
    this->seed = seed;
    generator.seed(seed);
}

Field::~Field()
{
    delete [] field;
}

Cell *Field::at(int row, int column)
{
    return field[row][column];
}

void Field::setPlayer(int row, int col)
{
    field[player.first][player.second]->setPlayer(false);
    if (traced){
        if (!field[row][col]->hasTrace)
            field[player.first][player.second]->setTrace(true);
        else
            field[player.first][player.second]->setTrace(false);
    }
    player.first = row;
    player.second = col;
    field[player.first][player.second]->setPlayer(true);
}

void Field::moveUp()
{
    if (player.first == 0|| (field[player.first][player.second]->borders & Cell::UP))
        return;
    setPlayer(player.first - 1, player.second);
}

void Field::moveDown()
{
    if (player.first == rows - 1|| (field[player.first][player.second]->borders & Cell::DOWN))
        return;
    setPlayer(player.first + 1, player.second);
}

void Field::moveLeft()
{
    if (player.second == 0|| (field[player.first][player.second]->borders & Cell::LEFT))
        return;
    setPlayer(player.first, player.second - 1);
}

void Field::moveRight()
{
    if (player.second == columns - 1 || (field[player.first][player.second]->borders & Cell::RIGHT))
        return;
    setPlayer(player.first, player.second + 1);
}

void Field::genStartFinish()
{
    this->player = {random()%rows*0.2, random()%columns};
    this->finish = {rows - random()%rows*0.2 - 1, columns - random()%columns - 1};
    setPlayer(player.first, player.second);
    field[finish.first][finish.second]->setFinish(true);
}

// path finding algorithm (BFS)
void Field::trace()
{
    traced = !traced;
    if (!traced){
        for(int i=0; i<rows; i++)
            for(int j=0; j<columns; j++)
                field[i][j]->setTrace(false);
        return;
    }
    qDebug() << "Tracing path";
    QStack<QPair<int, int>> stack;
    stack.append({player.first, player.second});
    bool ** checked = new bool* [rows];
    for (int i=0; i<rows; i++)
        checked[i] = new bool [columns];
    for(int i=0; i<rows; i++)
        for(int j=0; j<columns; j++)
            checked[i][j] = false;
    while(!stack.empty()){
        QPair<int, int> cell = stack.top();
        if (field[cell.first][cell.second]->hasFinish)
            break;
        checked[cell.first][cell.second] = true;
        QList<QPair<int, int>> neighbours;
        if (cell.first < rows && !(field[cell.first][cell.second]->borders&Cell::DOWN) && !checked[cell.first + 1][cell.second])
            neighbours.append({{cell.first + 1},{cell.second}});
        if (cell.second > 0 && !(field[cell.first][cell.second]->borders&Cell::LEFT) && !checked[cell.first][cell.second - 1])
            neighbours.append({{cell.first},{cell.second - 1}});
        if (cell.first > 0 && !(field[cell.first][cell.second]->borders&Cell::UP) && !checked[cell.first - 1][cell.second])
            neighbours.append({{cell.first - 1},{cell.second}});
        if (cell.second < columns && !(field[cell.first][cell.second]->borders&Cell::RIGHT) && !checked[cell.first][cell.second + 1])
            neighbours.append({{cell.first},{cell.second + 1}});
        if (!neighbours.empty())
            stack.append(neighbours[0]);
        else
            stack.pop();
    }

    while(!stack.empty()){
       QPair<int, int> cell = stack.pop();
       field[cell.first][cell.second]->setTrace(true);
    }
    delete [] checked;
}

QList<QPair<int, int>>* Field::findNeighbours(int x, int y, bool isClosedCell){
    QList<QPair<int, int>> *neighbours = new QList<QPair<int, int>>;
    if (y > 0 && ((field[x][y-1]->borders == 15 && isClosedCell) || (!isClosedCell && field[x][y-1]->borders != 15))){
        neighbours->append({{x},{y-1}});
    }
    if (y < columns - 1 && ((field[x][y+1]->borders == 15 && isClosedCell) || (!isClosedCell && field[x][y+1]->borders != 15))){
        neighbours->append({{x},{y+1}});
    }
    if (x > 0 && ((field[x-1][y]->borders == 15 && isClosedCell) || (!isClosedCell && field[x-1][y]->borders != 15))){
        neighbours->append({{x-1},{y}});
    }
    if (x < rows - 1 && ((field[x+1][y]->borders == 15 && isClosedCell) || (!isClosedCell && field[x+1][y]->borders != 15))){
        neighbours->append({{x+1},{y}});
    }
    return neighbours;
}

int Field::random()
{
    return generator() >> 1;
};

void Field::genPrim()
{
    qInfo() << "Prim";
    int x = random() % rows,
        y = random() % columns;
    QList<QPair<QPair<int, int>, QPair<int, int>>> borders;
    if (x != 0) borders.append({{x, y}, {x-1, y}});
    if (x != rows-1) borders.append({{x, y}, {x+1, y}});
    if (y != 0) borders.append({{x, y}, {x, y-1}});
    if (y != columns-1) borders.append({{x, y}, {x, y+1}});

    while (!borders.empty()){
        int pos = random()%borders.size();
        QPair<QPair<int, int>, QPair<int, int>> border = borders[pos];
        int row1 = border.first.first, col1 = border.first.second, row2 = border.second.first, col2 = border.second.second;
        borders.removeAt(pos);
        if (field[row2][col2]->borders == 15){

            int drow = border.second.first - border.first.first;
            int dcol = border.second.second - border.first.second;
            if (drow == 0 && dcol == 1){
                field[row1][col1]->removeBorder(Cell::RIGHT);
                field[row2][col2]->removeBorder(Cell::LEFT);
            }
            if (drow == 0 && dcol == -1){
                field[row1][col1]->removeBorder(Cell::LEFT);
                field[row2][col2]->removeBorder(Cell::RIGHT);
            }
            if (drow == 1 && dcol == 0){
                field[row1][col1]->removeBorder(Cell::DOWN);
                field[row2][col2]->removeBorder(Cell::UP);
            }
            if (drow == -1 && dcol == 0){
                field[row1][col1]->removeBorder(Cell::UP);
                field[row2][col2]->removeBorder(Cell::DOWN);
            }
            if ((field[row2][col2]->borders & Cell::UP) && row2 > 0){
                borders.append({{row2, col2}, {row2-1, col2}});
            }
            if ((field[row2][col2]->borders & Cell::DOWN) && row2 < rows-1){
                borders.append({{row2, col2}, {row2+1, col2}});
            }
            if ((field[row2][col2]->borders & Cell::RIGHT) && col2 > 0){
                borders.append({{row2, col2}, {row2, col2-1}});
            }
            if ((field[row2][col2]->borders & Cell::LEFT) && col2 < columns-1){
                borders.append({{row2, col2}, {row2, col2+1}});
            }
        }
    }

    genStartFinish();
}

void Field::genKruskal()
{
    qInfo() << "Kruskal";
    QVector<QPair<QPair<int, int>, QPair<int, int>>> borders;
    for(int row=0; row<rows; row++){
        for(int col=0; col<columns-1; col++){
            borders.append({{row, col}, {row, col+1}});
        }
    }
    for(int col=0; col<columns; col++){
        for(int row=0; row<rows-1; row++){
            borders.append({{row, col}, {row+1, col}});
        }
    }
    std::shuffle(borders.begin(), borders.end(), generator);

    int ** ids = new int* [rows];
    for(int i=0; i<rows; i++){
        ids[i] = new int [columns];
    }
    for(int row=0; row<rows; row++){
        for(int col=0; col<columns; col++){
            ids[row][col]=-1;
        }
    }

    int minID = 0;
    QVector<int> parents;
    std::function<int(int)> find, depth;
    find = [&parents, &find](int id) {
        return (id == -1) ? -1 : ((parents[id] == -1) ? id : find(parents[id]));
    };
    depth = [&parents, &depth](int id) {
        return (id == -1) ? 0 : (depth(parents[id]) + 1);
    };

    auto delBorders = [this](int row1, int col1, int row2, int col2) {
        if (col1 + 1 == col2){
            field[row1][col1]->removeBorder(Cell::RIGHT);
            field[row2][col2]->removeBorder(Cell::LEFT);
        }
        else{
            field[row1][col1]->removeBorder(Cell::DOWN);
            field[row2][col2]->removeBorder(Cell::UP);
        }
    };

    for (auto border : borders)
    {
        int &row1 = border.first.first, &col1 = border.first.second, &row2 = border.second.first, &col2 = border.second.second;
        int id1 = ids[row1][col1], id2 = ids[row2][col2];
        if (find(id1) == find(id2) && find(id1) != -1){
            continue;
        }
        if (find(id1) == find(id2) && find(id1) == -1){
            ids[row1][col1] = minID;
            ids[row2][col2] = minID;
            parents.append(-1);
            minID++;
            delBorders(row1, col1, row2, col2);
            continue;
        }
        if (find(id1) == -1){
            ids[row1][col1] = ids[row2][col2];
            delBorders(row1, col1, row2, col2);
            continue;
        }
        if (find(id2) == -1){
            ids[row2][col2] = ids[row1][col1];
            delBorders(row1, col1, row2, col2);
            continue;
        }
        if (depth(id1) < depth(id2))
            parents[find(id1)] = find(id2);
        else
            parents[find(id2)] = find(id1);
        delBorders(row1, col1, row2, col2);
    }
    delete [] ids;

    genStartFinish();
}

void Field::genHuntAndKill()
{
    qInfo() << "Hunt-and-Kill";
    int x = 0, y = 0;
    QPair<int, int> scan = {0, 0};
    scan.second = 0;

    std::function<bool()> scanInc = [&scan, this]() mutable {
      if (scan.first == rows - 1 && scan.second == columns - 1)
          return false;
      if (scan.second != columns - 1){
          scan.second++;
          return true;
      }
      scan.second = 0;
      scan.first++;
      return true;
    };

    while(scan.first != rows-1 && scan.second != columns){
        QList<QPair<int, int>> *moveCandidates = findNeighbours(x, y, true);
        if (moveCandidates->size() != 0){
            QPair<int, int> toCell = moveCandidates->at(random() % moveCandidates->size());
            delete moveCandidates;

            int dx = toCell.first - x, dy = toCell.second - y;
            if (dy == -1){
                field[x][y]->removeBorder(Cell::LEFT);
                field[x][y-1]->removeBorder(Cell::RIGHT);
            }
            if (dx == 1){
                field[x][y]->removeBorder(Cell::DOWN);
                field[x+1][y]->removeBorder(Cell::UP);
            }
            if (dy == 1){
                field[x][y]->removeBorder(Cell::RIGHT);
                field[x][y+1]->removeBorder(Cell::LEFT);
            }
            if (dx == -1){
                field[x][y]->removeBorder(Cell::UP);
                field[x-1][y]->removeBorder(Cell::DOWN);
            }
            x+=dx;
            y+=dy;
            continue;
        }
        else{
            delete moveCandidates;
            while(true){
                QList<QPair<int, int>> *emptyNeighbours = findNeighbours(scan.first, scan.second, true);
                int N = emptyNeighbours->size();
                delete emptyNeighbours;
                if (N!=0){
                    x = scan.first;
                    y = scan.second;
                    break;
                }
                if(!scanInc())
                    break;
            }
        }
    }

    genStartFinish();
}

void Field::genRecursiveBacktracker()
{
    qInfo() << "Recursive Backtracker";
    QStack<QPair<int, int>> stack;
    stack.append({random() % rows, random() % columns});
    while(!stack.empty()){
        QPair<int, int> cell = stack.top();
        QList<QPair<int, int>>* neighbours = findNeighbours(cell.first, cell.second, true);
        if (neighbours->size() != 0){
            QPair<int, int> toCell = neighbours->at(random() % neighbours->size());
            delete neighbours;
            int x = cell.first, y = cell.second;
            int dx = toCell.first - x, dy = toCell.second - y;
            if (dy == -1){
                field[x][y]->removeBorder(Cell::LEFT);
                field[x][y-1]->removeBorder(Cell::RIGHT);
            }
            if (dx == 1){
                field[x][y]->removeBorder(Cell::DOWN);
                field[x+1][y]->removeBorder(Cell::UP);
            }
            if (dy == 1){
                field[x][y]->removeBorder(Cell::RIGHT);
                field[x][y+1]->removeBorder(Cell::LEFT);
            }
            if (dx == -1){
                field[x][y]->removeBorder(Cell::UP);
                field[x-1][y]->removeBorder(Cell::DOWN);
            }
            stack.append({x+dx, y+dy});
            continue;
        }
        else{
            delete neighbours;
            stack.pop();
        }
    }

    genStartFinish();
}

void Field::genRecursiveDivision()
{
    qInfo() << "Recursive Division";
    for(int row=0; row < rows; row++){
        for(int col = 0; col < columns; col++){
            if (row > 0) field[row][col]->removeBorder(Cell::UP);
            if (row < rows - 1) field[row][col]->removeBorder(Cell::DOWN);
            if (col > 0) field[row][col]->removeBorder(Cell::LEFT);
            if (col < columns - 1) field[row][col]->removeBorder(Cell::RIGHT);
        }
    }

    std::function<void(int, int, int, int)> divide;
    divide = [this, &divide](int x, int y, int h, int w){
        //qDebug() << x << y << h << w;
        if (h == w && h == 1)
            return;
        bool divideH = (h*h >= random()%(h*h+w*w) && h >= 2) || w == 1; // will we add horizontal line?
        if (divideH){
            int divPos = random()%(h-1);
            int gap = random()%w;
            for(int col = y; col < y + w; col++){
                if (col != y + gap){
                    field[x + divPos][col]->borders += Cell::DOWN;
                    field[x + divPos + 1][col]->borders += Cell::UP;
                }
            }
            divide(x, y, divPos + 1, w);
            divide(x + divPos + 1, y, h - (divPos + 1), w);
        }
        else{
            int divPos = random()%(w-1);
            int gap = random()%h;
            for(int row = x; row < x + h; row++){
                if (row != x + gap){
                    field[row][y + divPos]->borders += Cell::RIGHT;
                    field[row][y + divPos + 1]->borders += Cell::LEFT;
                }
            }
            divide(x, y, h, divPos+1);
            divide(x, y + divPos + 1, h, w - (divPos + 1));
        }
    };

    divide(0, 0, rows, columns);

    genStartFinish();
}

