

inline void PrintChessboardBorder()
{
    std::cout << "+";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << std::string(CELL_WIDTH, '-') << "+";
    }
    std::cout << std::endl;
}
inline void PrintChessboard() {
    // 打印棋盘的顶部边框
    PrintChessboardBorder();

    // 打印棋盘内容
    for (int i = 0; i < SIZE; ++i) {
        std::cout << "|";
        for (int j = 0; j < SIZE; ++j) {
            // 计算当前格子的数字
            int number = (i * SIZE + j + 1)*100;
            // 将数字转换为字符串
            std::string numberStr = std::to_string(number);
            int numberWidth = numberStr.length();

            // 计算左右填充的空格数量
            int leftPadding = (CELL_WIDTH - numberWidth) / 2;
            int rightPadding = CELL_WIDTH - numberWidth - leftPadding;

            // 打印数字并居中
            std::cout << std::string(leftPadding, ' ') << numberStr << std::string(rightPadding, ' ') << "|";
        }
        std::cout << std::endl;

        PrintChessboardBorder();
    }
}