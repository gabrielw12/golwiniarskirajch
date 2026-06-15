class Conway : public Rules {
    public:
        bool next_state(bool is_alive, int neighbours) const override {
            if (is_alive) {
                return (neighbours == 2 || neighbours == 3);
            }
            else {
                return (neighbours == 3);
            }
        }
};

//TODO funkcja licząca sąsiadów

//TODO funkcja krok symulacji

//TODO funkcja do wyświetlania planszy