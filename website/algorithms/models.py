from django.db import models
from django.urls import reverse
from django.core.files import File

# Create your models here.

class Algorithm(models.Model):
    name = models.CharField(max_length = 40, unique = True)
    author = models.CharField(max_length = 30)  # nick użytkownika serwisu
    alg_file = models.FileField(upload_to='zip_files/')
    counted = models.BooleanField(default = False)
    #score = models.FloatField()
    #rank = models.IntegerField()

    #boxPlotChart = models.ImageField(upload_to='charts/')


    def get_absolute_url(self):
        return reverse('algorithms:detail', kwargs={'pk' : self.pk})

    def init_charts_and_tables(self):
        pass
        #f = open('M_1_D2.txt', 'w')
        #myfile = File(f)
        #self.results10 = myfile
        #self.save()
        #return self

    def actualize_rank(self, new_rank):
        pass

    def __str__(self):
        return self.name

# gdzie umieścić ścieżki do zdjęć do odpowiednich wykresów?

#class Benchmark(models.Model):
#    name = models.CharField(max_length = 40)

#    def __str__(self):
#        return self.name

    #klasaPytonowa = biblioteka.funCPP

class Outcome(models.Model):
    algorithm = models.ForeignKey(Algorithm, on_delete = models.CASCADE)
    dimension = models.IntegerField()
    function = models.IntegerField()
    best = models.CharField(max_length = 30)
    worst = models.CharField(max_length = 30)
    median = models.CharField(max_length = 30)
    mean = models.FloatField()
    std = models.CharField(max_length = 30)

    def __str__(self):
        return self.algorithm.__str__() + ', fun: ' + str(self.function) + ', dim: ' + str(self.dimension)