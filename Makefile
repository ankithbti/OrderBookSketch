all:
	(cd MarketDataPublisher; make all)
	(cd OrderBookStore; make all)
	(cd OrderManagementSystem; make all)
clean:
	(cd MarketDataPublisher;make clean)
	(cd OrderBookStore;make clean)
	(cd OrderManagementSystem; make clean)
