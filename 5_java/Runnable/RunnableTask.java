class RunnableTask implements Runnable {
    private final Obraz obraz;
    private final int startIndex;
    private final int endIndex;
    private final int threadId;

    public RunnableTask(Obraz obraz, int startIndex, int endIndex, int threadId) {
        this.obraz = obraz;
        this.startIndex = startIndex;
        this.endIndex = endIndex;
        this.threadId = threadId;
    }

    @Override
    public void run() {
        // histogram dla danego zakresu
        obraz.calculateHistogramBlock(startIndex, endIndex);

        // wyniki wątku
        obraz.printPartialHistogram(startIndex, endIndex, threadId);
    }
}
