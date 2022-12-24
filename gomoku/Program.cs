/**
 *
 * The goal is to scale up the board using alpha beta pruning
 * and use a data structure that minimize allocation
 *
 * We prebuild a data structure that maps a cell to the set of lines it has
 * so that during game play, we can update the per line statistics quickly
 * 
 * The lines are arranged in a priority queue so that we know which line is 
 * most likely going to win.
 * 
 */
namespace gomoku
{
    using System.Diagnostics;
    using System.Linq;

    internal class Board
    {
        public const int WIDTH = 15;
        public const int HEIGHT = 15;
        public const int AREA = WIDTH * HEIGHT;
        public const int MAX_LINES = 20;

        /**
         * distance[cell] = minimum distance to a placed piece
         */
        private int[] distances;

        /**
         * numLines[cell] = number of lines contains this cell
         */
        private int[] numLines;

        /**
         * lines[cell, [0, numLines[cell])] are the lines that contain the cell
         */
        private int[,] lines;

        /**
         * countsByLine[line, player] = number of pieces a player has on that line
         */
        private int[,] countsByLine;

        /**
         * queues[,player] is a priority queue of lines for the player, the most likely to win line has the highest priority
         */
        private int[,] queues;

        /**
         * queueIndex[line, player] is the queue position of the line for the player
         */
        private int[,] queueIndex;

        /**
         * bucketEnds[bucketValue,player] is where the (exclusive) bucket ends for a particular bucket value,
         * it is also implicitly the start for the next bucket start.
         */
        private int[,] bucketEnds;

        /**
         * pieces[row, col] = the piece on row, col
         */
        private int[,] pieces;

        /**
         * count = number of pieces placed on the board
         */
        private int count;

        /**
         * totalLines = total number of lines;
         */
        private int totalLines;

        public Board()
        {
            this.distances = new int[AREA];
            for (int i = 0; i < AREA; i++)
            {
                this.distances[i] = AREA;
            }

            this.numLines = new int[AREA];
            this.lines = new int[AREA, MAX_LINES];

            int lineNumber = 0;
            for (int row = 0; row < HEIGHT; row++)
            {
                for (int column = 0; column + 4 < WIDTH; column++)
                {
                    int startCell = row * WIDTH + column;
                    int endCell = row * WIDTH + (column + 5);
                    for (int cell = startCell; cell < endCell; cell++)
                    {
                        this.lines[cell, this.numLines[cell]++] = lineNumber;
                    }
                    lineNumber++;
                }
            }
            for (int column = 0; column < WIDTH; column++)
            {
                for (int row = 0; row + 4 < HEIGHT; row++)
                {
                    int startCell = row * WIDTH + column;
                    int endCell = (row + 5) * WIDTH + column;
                    for (int cell = startCell; cell < endCell; cell += WIDTH)
                    {
                        this.lines[cell, this.numLines[cell]++] = lineNumber;
                    }
                    lineNumber++;
                }
            }
            for (int row = 0; row + 4 < HEIGHT; row++)
            {
                for (int column = 0; column + 4 < WIDTH; column++)
                {
                    int startCell = row * WIDTH + column;
                    int endCell = (row + 5) * WIDTH + (column + 5);
                    for (int cell = startCell; cell < endCell; cell += WIDTH + 1)
                    {
                        this.lines[cell, this.numLines[cell]++] = lineNumber;
                    }
                    lineNumber++;
                }
            }
            for (int row = 0; row + 4 < HEIGHT; row++)
            {
                for (int column = 4; column < WIDTH; column++)
                {
                    int startCell = row * WIDTH + column;
                    int endCell = (row + 5) * WIDTH + (column - 5);
                    for (int cell = startCell; cell < endCell; cell += (WIDTH - 1))
                    {
                        this.lines[cell, this.numLines[cell]++] = lineNumber;
                    }
                    lineNumber++;
                }
            }
            this.totalLines = lineNumber;
            this.countsByLine = new int[this.totalLines, 2];
            this.queues = new int[this.totalLines, 2];
            this.queueIndex = new int[this.totalLines, 2];
            for (int p = 0; p < 2; p++)
            {
                for (int i = 0; i < totalLines; i++)
                {
                    this.queues[i, p] = this.queueIndex[i, p] = i;
                }
            }
            this.bucketEnds = new int[7, 2];
            this.bucketEnds[0, 0] = this.bucketEnds[0, 1] = this.totalLines;
            this.pieces = new int[HEIGHT, WIDTH];
        }

        public int[]? Place(int player, int row, int col)
        {
            if (this.pieces[row, col] != 0)
            {
                return null;
            }
            this.pieces[row, col] = player + 1;
            int cell = row * WIDTH + col;
            for (int i = 0; i < this.numLines[cell]; i++)
            {
                int line = this.lines[cell, i];
                int myCountWas = this.countsByLine[line, player];
                int theirCount = this.countsByLine[line, 1 - player];
                if (theirCount == 0)
                {
                    this.ChangePriority(line, player, myCountWas, myCountWas + 1);
                }
                else
                {
                    if (myCountWas == 0)
                    {
                        this.ChangePriority(line, 1 - player, theirCount, 0);
                    }
                }
                this.countsByLine[this.lines[cell, i], player]++;
            }

            // TODO: Consider pooling
            int[] oldDistances = this.distances;
            int[] newDistances = new int[AREA];
            cell = 0;
            for (int r = 0; r < HEIGHT; r++)
            {
                for (int c = 0; c < WIDTH; c++)
                {
                    newDistances[cell] = Math.Min(Math.Abs(r - row) + Math.Abs(c - col), this.distances[cell]);
                    cell++;
                }
            }
            this.distances = newDistances;
            this.count++;
            return oldDistances;
        }

        public void Undo(int player, int row, int col, int[] oldDistances)
        {
            this.pieces[row, col] = 0;
            int cell = row * WIDTH + col;
            for (int i = 0; i < this.numLines[cell]; i++)
            {
                int line = this.lines[cell, i];
                int myCountWas = this.countsByLine[line, player];
                int theirCount = this.countsByLine[line, 1 - player];
                if (theirCount == 0)
                {
                    this.ChangePriority(line, player, myCountWas, myCountWas - 1);
                }
                else
                {
                    if (myCountWas == 1)
                    {
                        this.ChangePriority(line, 1 - player, 0, theirCount);
                    }
                }
                this.countsByLine[this.lines[cell, i], player]--;
            }

            // TODO: Return to pool instead
            this.distances = oldDistances;
        }

        private void ChangePriority(int line, int player, int from, int to)
        {
            while (from < to)
            {
                int linePosition = this.queueIndex[line, player];
                Debug.Assert(linePosition < this.bucketEnds[from, player]);
                Debug.Assert(this.bucketEnds[from + 1, player] <= linePosition);
                Swap(linePosition, this.bucketEnds[from + 1, player], player);
                this.bucketEnds[from + 1, player]++;
                from++;
            }
            while (from > to)
            {
                int linePosition = this.queueIndex[line, player];
                Debug.Assert(linePosition < this.bucketEnds[from, player]);
                Debug.Assert(this.bucketEnds[from + 1, player] <= linePosition);
                Swap(linePosition, this.bucketEnds[from, player] - 1, player);
                this.bucketEnds[from, player]--;
                from--;
            }
        }

        private void Swap(int position1, int position2, int player)
        {
            int line1 = this.queues[position1, player];
            int line2 = this.queues[position2, player];
            this.queues[position1, player] = line2;
            this.queues[position2, player] = line1;
            this.queueIndex[line1, player] = position2;
            this.queueIndex[line2, player] = position1;
        }

        public void Display()
        {
            Console.Clear();
            for (int c = 0; c < WIDTH; c++)
            {
                Console.Write("+-");
            }
            Console.WriteLine("+");
            for (int r = 0; r < HEIGHT; r++)
            {
                for (int c = 0; c < WIDTH; c++)
                {
                    Console.Write("|");
                    Console.Write(this.Sign(this.pieces[r, c]));
                }
                Console.WriteLine("|");
                for (int c = 0; c < WIDTH; c++)
                {
                    Console.Write("+-");
                }
                Console.WriteLine("+");
            }
        }

        private char Sign(int p)
        {
            switch (p)
            {
                case 0: return ' ';
                case 1: return 'O';
                case 2: return 'X';
                default: throw new Exception("Should not happen");
            }
        }

        public bool GameOver()
        {
            return (bucketEnds[5, 0] > 0) || (bucketEnds[5, 1] > 0) || (this.count == AREA);
        }

        public string? Winner()
        {
            if (bucketEnds[5, 0] > 0)
            {
                return "Player";
            }
            else if (bucketEnds[5, 1] > 0)
            {
                return "Computer";
            }
            else if (this.count == AREA)
            {
                return "Draw";
            }
            else
            {
                return null;
            }
        }

        public void ComputerMoves()
        {
            var (bestScore, bestMove) = Minimax(1, -1, -1, 3);
            int r = bestMove / WIDTH;
            int c = bestMove % WIDTH;
            this.Place(1, r, c);
        }

        public (int, int) Minimax(int player, int alpha, int beta, int depth)
        {
            if (depth == 0)
            {
                int bestPlayer = 0;
                int bestComputer = 0;
                for (int i = 5; i >= 0; i--)
                {
                    if (bucketEnds[i,0] > 0) {
                        bestPlayer = i;
                        break;
                    }
                }
                for (int i = 5; i >= 0; i--)
                {
                    if (bucketEnds[i,1] > 0) {
                        bestComputer = i;
                        break;
                    }
                }
                if (bestPlayer == 5)
                {
                    return (5, -1);
                }
                else if (bestComputer == 5)
                {
                    return (15, -1);
                }
                else
                {
                    return (10 + bestComputer - bestPlayer, -1);
                }
            }
            int count = 0;
            int[] choices = new int[AREA];
            int cell = 0;
            for (int r = 0; r < HEIGHT; r++)
            {
                for (int c = 0; c < WIDTH; c++)
                {
                    if (this.pieces[r, c] == 0)
                    {
                        choices[count++] = cell;
                    }
                    cell++;
                }
            }
            // TODO: Avoid Linq
            int[] selections = choices.Take(count).OrderBy(x => distances[x]).ToArray();
            int bestMove = -1;
            int bestScore = -1;
            for (int i = 0; i < selections.Length; i++)
            {
                int selection = selections[i];
                int r = selection / WIDTH;
                int c = selection % WIDTH;
                int[] oldDistances = this.Place(player, r, c)!;
                var (score, _) = this.Minimax(1 - player, alpha, beta, depth - 1);
                if (player == 0)
                {
                    if ((bestScore == -1) || (score < bestScore))
                    {
                        bestScore = score;
                        bestMove = selection;
                    }
                    if ((alpha != -1) && (bestScore <= alpha))
                    {
                        this.Undo(player, r, c, oldDistances);
                        return (bestScore, bestMove);
                    }
                    if ((beta == -1) || (beta > bestScore))
                    {
                        beta = bestScore;
                    }
                }
                else
                {
                    if ((bestScore == -1) || (score > bestScore))
                    {
                        bestScore = score;
                        bestMove = selection;
                    }
                    if ((beta != -1) && (bestScore >= beta))
                    {
                        this.Undo(player, r, c, oldDistances);
                        return (bestScore, bestMove);
                    }
                    if ((alpha == -1) || (alpha < bestScore))
                    {
                        alpha = bestScore;
                    }
                }
                this.Undo(player, r, c, oldDistances);
            }
            return (bestScore, bestMove);
        }

    }

    internal static class Program
    {
        private static int Main(string[] args)
        {
            Board board = new Board();
            board.Display();
            int player = 0;
            while (true)
            {
                if (player == 0)
                {
                    while (true)
                    {
                        Console.Write("Enter row: ");
                        int r = int.Parse(Console.ReadLine());
                        Console.Write("Enter col: ");
                        int c = int.Parse(Console.ReadLine());
                        if (board.Place(player, r, c) != null)
                        {
                            break;
                        }
                    }
                    board.Display();
                }
                else
                {
                    board.ComputerMoves();
                    board.Display();
                }
                player = 1 - player;
                if (board.GameOver())
                {
                    Console.WriteLine(board.Winner());
                    break;
                }
            }
            return 0;
        }
    }
}
