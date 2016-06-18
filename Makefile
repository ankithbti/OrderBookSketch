all:
	(cd MarketDataPublisher; make all)
	(cd OrderBookStore; make all)
clean:
	(cd MarketDataPublisher;make clean)
	(cd OrderBookStore;make clean)
