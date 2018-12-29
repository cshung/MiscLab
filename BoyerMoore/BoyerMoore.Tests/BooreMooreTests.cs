using Xunit;

namespace BoyerMoore.Tests
{
    public class BooreMooreTests
    {
        [Fact]
        public void HelloWorld()
        {
            Assert.Equal(new int[] { 6 }, BoyerMoore.IndexOf("Hello World", "World"));
        }

        [Fact]
        public void MatchSuffixAndShift()
        {
            Assert.Equal(new int[] { 2 }, BoyerMoore.IndexOf("pppqpq", "pqpq"));
        }

        [Fact]
        public void MatchOverlapPattern()
        {
            Assert.Equal(new int[] { 0, 2 }, BoyerMoore.IndexOf("pqpqpq", "pqpq"));
        }
    }
}
