namespace gomoku
{
    using System;
    using System.Linq;
    using System.Collections.Generic;

    internal class Placement
    {
        public List<Tuple<int, int>> Removed { get; set; }
        public List<Tuple<int, int>> Added { get; set; }
        public int Player { get; set; }
        public int Row { get; set; }
        public int Col { get; set; }
    }

    internal class Board
    {
        public const int WIDTH = 8;
        public const int HEIGHT = 8;
        private static char[] piece = new char[] { ' ', 'o', 'x' };
        private int[,,] sequence = new int[WIDTH, HEIGHT, 4];

        private int[,] board;
        private int[,] chainsByBegin;
        private int[,] chainsByEnd;
        private int score;
        private int completedCount;

        public Board()
        {
            this.board = new int[WIDTH, HEIGHT];
        }

        public void Display(bool clear = true)
        {
            if (clear) { Console.Clear(); }
            for (int i = 0; i < HEIGHT; i++)
            {
                Console.Write("+");
                for (int j = 0; j < WIDTH; j++)
                {
                    Console.Write("-+");
                }
                Console.WriteLine();

                Console.Write("|");
                for (int j = 0; j < WIDTH; j++)
                {
                    Console.Write(piece[board[i, j]]);
                    Console.Write('|');
                }

                Console.WriteLine();
            }
            Console.Write("+");
            for (int j = 0; j < WIDTH; j++)
            {
                Console.Write("-+");
            }
            Console.WriteLine();
        }

        private int Transform(int length)
        {
            switch(length)
            {
                case 1: return 0;
                case 2: return 1;
                case 3: return 3;
                case 4: return 7;
            }

            return 10086;
        }

        public int Evaluate()
        {
            return score;
        }

        public void BuildSequence()
        {
            int sequenceNumber = 1;
            for (int row = 0; row < HEIGHT; row++)
            {
                for (int col = 0; col < WIDTH; col++)
                {
                    sequence[row, col, 0] = sequenceNumber++;
                }
                sequenceNumber++;
            }
            for (int col = 0; col < WIDTH; col++)
            {
                for (int row = 0; row < HEIGHT; row++)
                {
                    sequence[row, col, 1] = sequenceNumber++;
                }
                sequenceNumber++;
            }
            for (int i = HEIGHT - 1; i >= 0; i--)
            {
                int row = i;
                int col = 0;
                while (row < HEIGHT && col < WIDTH)
                {
                    sequence[row, col, 2] = sequenceNumber++;
                    row++;
                    col++;
                }
                sequenceNumber++;
            }
            for (int j = 1; j < WIDTH; j++)
            {
                int row = 0;
                int col = j;
                while (row < HEIGHT && col < WIDTH)
                {
                    sequence[row, col, 2] = sequenceNumber++;
                    row++;
                    col++;
                }
                sequenceNumber++;
            }
            for (int j = 0; j < WIDTH; j++)
            {
                int row = 0;
                int col = j;
                while (row < HEIGHT && col >= 0)
                {
                    sequence[row, col, 3] = sequenceNumber++;
                    row++;
                    col--;
                }
                sequenceNumber++;
            }
            for (int i = 1; i < HEIGHT; i++)
            {
                int row = i;
                int col = WIDTH - 1;
                while (row < HEIGHT && col >= 0)
                {
                    sequence[row, col, 3] = sequenceNumber++;
                    row++;
                    col--;
                }
                sequenceNumber++;
            }
            this.chainsByBegin = new int[2, sequenceNumber];
            this.chainsByEnd = new int[2, sequenceNumber];
        }

        public bool IsAvailable(int row, int col)
        {
            return this.board[row, col] == 0;
        }

        public Placement Place(int row, int col, int player)
        {
            this.board[row, col] = player;

            player--;

            Placement placement = new Placement
            {
                Row = row,
                Col = col,
                Removed = new List<Tuple<int, int>>(),
                Added = new List<Tuple<int, int>>(),
                Player = player
            };

            for (int i = 0; i < 4; i++)
            {
                int humanNumber = sequence[row, col, i];
                Tuple<int, int> before = Tuple.Create(chainsByEnd[player, humanNumber - 1], humanNumber - 1);
                Tuple<int, int> after = Tuple.Create(humanNumber + 1, chainsByBegin[player, humanNumber + 1]);

                bool hasBefore = before.Item1 != 0;
                bool hasAfter =  after.Item2 != 0;

                if (hasBefore)
                {
                    placement.Removed.Add(before);
                    if (hasAfter)
                    {
                        placement.Removed.Add(after);
                        placement.Added.Add(Tuple.Create(before.Item1, after.Item2));
                    }
                    else
                    {
                        placement.Added.Add(Tuple.Create(before.Item1, humanNumber));
                    }
                }
                else if (hasAfter)
                {
                    placement.Removed.Add(after);
                    placement.Added.Add(Tuple.Create(humanNumber, after.Item2));
                }
                else
                {
                    placement.Added.Add(Tuple.Create(humanNumber, humanNumber));
                }
            }

            foreach (var toRemove in placement.Removed)
            {
                int length = toRemove.Item2 - toRemove.Item1 + 1;
                if (length == 5)
                {
                    completedCount--;
                }
                score -= (player * 2 - 1) * Transform(length);
                chainsByBegin[player, toRemove.Item1] = 0;
                chainsByEnd[player, toRemove.Item2] = 0;
            }

            foreach (var toAdd in placement.Added)
            {
                int length = toAdd.Item2 - toAdd.Item1 + 1;
                if (length == 5)
                {
                    completedCount++;
                }
                score += (player * 2 - 1) * Transform(length);
                chainsByBegin[player, toAdd.Item1] = toAdd.Item2;
                chainsByEnd[player, toAdd.Item2] = toAdd.Item1;
            }

            return placement;
        }

        public void Undo(Placement placement)
        {
            int player = placement.Player;
            this.board[placement.Row, placement.Col] = 0;
            foreach (var toRemove in placement.Added)
            {
                int length = toRemove.Item2 - toRemove.Item1 + 1;
                if (length == 5)
                {
                    completedCount--;
                }
                score -= (player * 2 - 1) * Transform(length);
                chainsByBegin[player, toRemove.Item1] = 0;
                chainsByEnd[player, toRemove.Item2] = 0;
            }

            foreach (var toAdd in placement.Removed)
            {
                int length = toAdd.Item2 - toAdd.Item1 + 1;
                if (length == 5)
                {
                    completedCount++;
                }
                score += (player * 2 - 1) * Transform(length);
                chainsByBegin[player, toAdd.Item1] = toAdd.Item2;
                chainsByEnd[player, toAdd.Item2] = toAdd.Item1;
            }

        }

        public bool IsCompleted()
        {
            return completedCount > 0;
        }
    }

    internal static class Program
    {
        private static int FindComputerMove(Board board, int depth, int beta, out int bestRow, out int bestCol)
        {
            if (board.IsCompleted())
            {
                bestRow = bestCol = 0;
                return board.Evaluate();
            }

            bool hasCandidate = false;
            int bestValue = int.MinValue;
            bestRow = -1;
            bestCol = -1;

            for (int row = 0; row < Board.WIDTH; row++)
            {
                for (int col = 0; col < Board.HEIGHT; col++)
                {
                    if (board.IsAvailable(row, col))
                    {
                        var placement = board.Place(row, col, 2);
                        int value = depth > 0 ? FindHumanMove(board, depth - 1, bestValue) : board.Evaluate();
                        board.Undo(placement);

                        if (!hasCandidate || value > bestValue)
                        {
                            bestValue = value;
                            bestRow = row;
                            bestCol = col;
                            hasCandidate = true;
                        }
                        if (value >= beta)
                        {
                            return value;
                        }
                    }
                }
            }
            return bestValue;
        }

        private static int FindHumanMove(Board board, int depth, int alpha)
        {
            if (board.IsCompleted())
            {
                return board.Evaluate();
            }

            bool hasCandidate = false;
            int bestValue = int.MaxValue;
            int bestRow = -1;
            int bestCol = -1;

            for (int row = 0; row < Board.WIDTH; row++)
            {
                for (int col = 0; col < Board.HEIGHT; col++)
                {
                    if (board.IsAvailable(row, col))
                    {
                        var placement = board.Place(row, col, 1);
                        int dummyRow;
                        int dummyCol;
                        int value = depth > 0 ? FindComputerMove(board, depth - 1, bestValue, out dummyRow, out dummyCol) : board.Evaluate();
                        board.Undo(placement);
                        if (!hasCandidate || value < bestValue)
                        {
                            bestValue = value;
                            bestRow = row;
                            bestCol = col;
                            hasCandidate = true;
                        }
                        if (bestValue <= alpha)
                        {
                            return bestValue;
                        }
                    }
                }
            }
            return bestValue;
        }

        private static void Main(string[] args)
        {
            Board board = new Board();
            board.BuildSequence();
            bool humanTurn = true;
            while (true)
            {
                board.Display();
                if (board.IsCompleted())
                {
                    break;
                }

                if (humanTurn)
                {
                    int row;
                    int col;
                    Console.Write("Enter the row: ");
                    string rowString = Console.ReadLine();
                    if (!int.TryParse(rowString, out row))
                    {
                        continue;
                    }
                    if (row < 1 || row > Board.HEIGHT)
                    {
                        continue;
                    }
                    Console.Write("Enter the col: ");
                    string colString = Console.ReadLine();
                    if (!int.TryParse(colString, out col))
                    {
                        continue;
                    }
                    if (col < 1 || col > Board.WIDTH)
                    {
                        continue;
                    }
                    if (!board.IsAvailable(row - 1, col - 1))
                    {
                        continue;
                    }
                    board.Place(row - 1, col - 1, 1);
                    humanTurn = false;
                }
                else
                {
                    int bestRow;
                    int bestCol;
                    FindComputerMove(board, 3, int.MaxValue, out bestRow, out bestCol);
                    board.Place(bestRow, bestCol, 2);
                    humanTurn = true;
                }
            }
        }

    }
}
