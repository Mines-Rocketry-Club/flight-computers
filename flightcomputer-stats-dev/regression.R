# Fits a curve to a set of falloff points for the BGA2869 "wideband" amplifer
library(drc)
library(ggplot2)
data <- structure(
  list(
    f=c(
      250,
      950,
      2150
    ),
    a=c(
      31.3,
      31.7,
      33.6
    )
  ),
  .Names = c("f", "a"),
  class = "data.frame",
  row.names = c(NA, -3L)
)
MM.model <- drm(a~f, data=data, fct=MM.2())
mmdf <- data.frame(f=seq(0,max(data$f),length.out=100)) #a=seq(0,max(data$a),length.out=100))
mmdf$v <- predict(MM.model, newdata=mmdf)
p <- ggplot(data, aes(x = f, y = a))
p <- p + theme_bw()
p <- p + xlab("Frequency")
p <- p + ylab("Gain")
p <- p + ggtitle("Frequency-Gain for the BGA2869")
p <- p + geom_point(alpha = 0.5)
#p <- p + geom_line(data = mmdf, aes(,f), colour = "green")
print(p)